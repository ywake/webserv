#include "cgi.hpp"
#include "http_define.hpp"
#include "parse_cgi_utils.hpp"
#include "response_message.hpp"
#include "webserv_utils.hpp"

#include <cerrno>
#include <cstring>
#include <unistd.h>

Cgi::Cgi(const http::RequestMessage &message) : cgi_builder_(message)
{
	if (pipe(pipe_to_cgi_) < 0) {
		throw std::runtime_error(std::string("pipe: ") + strerror(errno));
	}
}

void Cgi::SetRequestData(
	const std::string server_name, const std::string server_port, const std::string client_ip
)
{
	cgi_builder_.SetMetaVariables(server_name, server_port, client_ip);
}

ssize_t Cgi::WriteRequestData(size_t nbyte) const
{
	std::string body = cgi_builder_.GetHttpRequest().message_body_;
	return write(pipe_to_cgi_[WRITE], body.c_str(), nbyte);
}

Result<void> Cgi::Run(const std::string &cgi_path)
{
	std::vector<std::string> meta_variables = cgi_builder_.GetMetaVariables();
	std::string              script_path    = cgi_builder_.GetScriptPath();

	static const unsigned int kArgvSize = 2;

	const char *file = cgi_path.c_str();
	char       *argv[kArgvSize];
	char       *envp[meta_variables.size() + 1];

	argv[0] = const_cast<char *>(script_path.c_str());
	argv[1] = NULL;

	for (std::size_t i = 0; i < meta_variables.size(); i++) {
		envp[i] = const_cast<char *>(meta_variables[i].c_str());
	}
	envp[meta_variables.size()] = NULL;

	Result<int> res = StartCgiProcess(file, argv, envp);
	if (res.IsErr()) {
		return res.err;
	}
	pid_ = res.Val();
}

// non-blocking
bool Cgi::Terminate() const
{
	int status;

	int ret = waitpid(pid_, &status, WNOHANG);
	if (ret == -1) {
		return false;
	}
	if (ret == 0) {
		return true;
	}
	return WIFEXITED(status);
}

const Result<Cgi::PollInstructions> Cgi::Send()
{
	PollInstructions poll_instructions;

	// 全て書き込んだら、Resource.Receiveを発火させるpoll_instructionsを返す
	// 定数にする
	ssize_t written = WriteRequestData(100);
	if (written == -1) {
		return Error(errno);
	}
	if (written == 0) {
		// どうやってResource.Receiveを発火させる？
		// poll += PollInstruction(kAppendRead);
	}
	return Result<PollInstructions>();
}

Result<std::vector<char>> Cgi::Read(size_t nbyte) const
{
	// 定数にする
	std::vector<char> buffer(100);
	ssize_t           read_byte = read(pipe_to_cgi_[READ], buffer.data(), buffer.size());
	if (read_byte == -1) {
		return Error(errno);
	}
	buffer.resize(read_byte);
	return buffer;
}

const Result<Cgi::PollInstructions> Cgi::Receive()
{
	PollInstructions poll_instructions;

	if (read_buffer_.IsFull() && msg_buffer_.IsFull()) {
		// poll_instructions += PollInstructions(kTrimRead);
		return poll_instructions;
	}

	if (!read_buffer_.IsFull()) {
		Result<std::vector<char>> res = Read(100);
		if (res.IsErr()) {
			return res.err;
		}
		read_buffer_.push_back(res.Val());
		if (msg_buffer_.empty() && res.Val().empty()) {
			// poll_instructions += PollInstructions(kTrimRead);
		}
	}

	if (msg_buffer_.IsFull()) {
		// poll_instructions += PollInstructions(kAppendWrite);
		return poll_instructions;
	}

	Result<void> res = ParseCgiResponse();
	if (res.IsErr()) {
		return res.err;
	}
	// ここでメッセージバッファにhttpレスポンスを追加する方がスコープが揃うが、未完成のhttpレスポンスをResultでうまく返せない
	return poll_instructions;
}

const Result<void> Cgi::ParseCgiResponse()
{
	std::string line         = GetLine();
	bool        is_blankline = line == http::kCrLf || line == http::kNl;

	switch (state_) {
	case kParseHeader:
		http_builder_.AddHeader(line);
		if (is_blankline) {
			state_ = kParseBody;
		}
		break;

	case kParseBody:
		if (is_blankline) {
			return Error("Invalid Cgi Response.");
		}
		if (line.empty()) {
			state_ = kParseFinish;
		}
		http_builder_.AddBody(line);
		break;

	case kParseFinish:
	default:
		Result<http::ResponseMessage> res = http_builder_.Translate();
		if (res.IsErr()) {
			return res.err;
		}
		msg_buffer_.push_back(res.Val());
		state_ = kParseHeader; // 同じreceiveの中で２度呼ばれないことに依存する?
	}
	return Result<void>();
}

//[FIX]　ちゃんと一行返ってくる保証がない
std::string Cgi::GetLine()
{
	std::string line;

	// line = line_;
	while (true) {
		Emptiable<char> res = read_buffer_.GetChar();
		if (res.empty()) {
			break;
		}
		char c = res.Value();
		line += c;
		if (c == http::kNl[0]) {
			break;
			// line_.clear();
			// return line;
		}
	}
	// line_ += line;
	// return Emptiable<char>();
}

Result<int> Cgi::StartCgiProcess(const char *file, char **argv, char **envp) const
{
	int pipe_from_cgi[2];

	if (pipe(pipe_from_cgi) < 0) {
		return Error(errno);
	}

	pid_t pid = fork();
	switch (pid) {
	case -1:
		return Error(errno);
	case 0:
		Xdup2(pipe_to_cgi_[READ], STDIN_FILENO);
		Xdup2(pipe_from_cgi[WRITE], STDOUT_FILENO);
		Xclose(pipe_to_cgi_[READ]);
		Xclose(pipe_from_cgi[WRITE]);
		Xclose(pipe_to_cgi_[WRITE]);
		Xclose(pipe_from_cgi[READ]);

		execve(file, argv, envp);
		exit(EXIT_FAILURE);
	default:
		if (close(pipe_to_cgi_[READ]) == -1) {
			return Error(errno);
		}
		if (close(pipe_from_cgi[WRITE]) == -1) {
			return Error(errno);
		}
	}
	return pid;
}
