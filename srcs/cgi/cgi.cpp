#include "cgi.hpp"
#include "parse_cgi_utils.hpp"
#include "response_message.hpp"
#include "webserv_utils.hpp"

#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>

Cgi::Cgi(const http::RequestMessage &message)
	: message_(message), formdata_(message.request_line_.request_target_.GetRequestFormData())
{
	ExpSafetyPipe(pipe_to_cgi_);
	SearchScriptPath();
}

// MUSTなメタ変数
// https://wiki.suikawiki.org/n/CGI%E3%83%A1%E3%82%BF%E5%A4%89%E6%95%B0#section-%E3%83%A1%E3%82%BF%E5%A4%89%E6%95%B0%E3%81%AE%E4%B8%80%E8%A6%A7
void Cgi::SetMetaVariables(
	const std::string server_name, const std::string server_port, const std::string client_ip
)
{
	SetGateWayInterFace();
	SetQueryString();
	SetRequestMethod();
	SetServerProtocol();
	SetServerSoftWare();
	SetServerName(server_name);
	SetServerPort(server_port);
	SetRemoteAddr(client_ip);

	if (message_.HasMessageBody()) {
		SetContentLength();
	}
	if (message_.field_lines_.Contains("content-type")) {
		SetContentType();
	}
	SetScriptName();
	SetPathInfo();
}

std::vector<std::string> Cgi::GetMetaVariables() const
{
	return meta_variables_;
}

void Cgi::SearchScriptPath()
{
	std::vector<ThinString> hierarchy = Split(formdata_.path_, "/");

	for (std::vector<ThinString>::iterator it = hierarchy.begin(); it != hierarchy.end(); it++) {
		std::string lower_path = it->ToString();
		script_path_ += lower_path;
		if (!IsRegularFile(script_path_) || !EndsWith(lower_path, ".php")) {
			continue;
		}
		for (++it; it != hierarchy.end(); it++) {
			extra_path_ += it->ToString();
		}
	}
}

// FIX transfer-encofingヘッダでチャンク化の場合は、復号して本体の長さを決定しなければならない
void Cgi::SetContentLength()
{
	meta_variables_.push_back(message_.field_lines_.GetKeyValueString("content-length"));
}

void Cgi::SetContentType()
{
	meta_variables_.push_back(message_.field_lines_.GetKeyValueString("content-type"));
}

void Cgi::SetGateWayInterFace()
{
	meta_variables_.push_back("gateway-interface=CGI/1.1");
}

void Cgi::SetPathInfo()
{
	meta_variables_.push_back(MakeKeyValueString("path-info", extra_path_));
}

void Cgi::SetScriptName()
{
	meta_variables_.push_back(MakeKeyValueString("script-name", script_path_));
}

void Cgi::SetQueryString()
{
	meta_variables_.push_back(MakeKeyValueString("query-string", formdata_.query_.ToString()));
}

void Cgi::SetRemoteAddr(const std::string &remote_addr)
{
	meta_variables_.push_back(MakeKeyValueString("remote-addr", remote_addr));
}

void Cgi::SetRequestMethod()
{
	meta_variables_.push_back(
		MakeKeyValueString("request-method", message_.request_line_.method_.ToString())
	);
}

void Cgi::SetServerName(const std::string &server_name)
{
	meta_variables_.push_back(MakeKeyValueString("server-name", server_name));
}

void Cgi::SetServerPort(const std::string &server_port)
{
	meta_variables_.push_back(MakeKeyValueString("server-port", server_port));
}

void Cgi::SetServerProtocol()
{
	meta_variables_.push_back(
		MakeKeyValueString("server-protocol", message_.request_line_.http_version_.ToString())
	);
}

void Cgi::SetServerSoftWare()
{
	meta_variables_.push_back("server-software=webserv/1.0");
}

ssize_t Cgi::WriteRequestData(size_t nbyte) const
{
	return write(pipe_to_cgi_[WRITE], message_.message_body_.c_str(), nbyte);
}

void Cgi::Run(const std::string &cgi_path)
{
	static const unsigned int kArgvSize = 2;

	const char *file = cgi_path.c_str();
	char       *argv[kArgvSize];
	char       *envp[meta_variables_.size() + 1];

	argv[0] = const_cast<char *>(script_path_.c_str());
	argv[1] = NULL;

	for (std::size_t i = 0; i < meta_variables_.size(); i++) {
		envp[i] = const_cast<char *>(meta_variables_[i].c_str());
	}
	envp[meta_variables_.size()] = NULL;

	pid_ = StartCgiProcess(file, argv, envp);
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
	return Result<PollInstructions>(poll_instructions);
}

Result<std::vector<char>> Cgi::Read(size_t nbyte) const
{
	// 定数にする
	std::vector<char> buffer(100);
	ssize_t           read_byte = read(pipe_to_cgi_[READ], buffer.data(), buffer.size());
	if (read_byte == -1) {
		return Error("");
	}
	return buffer;
}

const Result<Cgi::PollInstructions> Cgi::Receive()
{
	PollInstructions poll_instructions;

	if (read_buffer_.IsFull() && msg_buffer_.IsFull()) {
		// poll_instructions += PollInstructions(kTrimWrite);
		return poll_instructions;
	}

	if (!read_buffer_.IsFull()) {
		Result<std::vector<char>> res = Read(100);
		if (res.IsErr()) {
			return Error("");
		}
		read_buffer_.push_back(res.Val());
		if (msg_buffer_.empty() && res.Val().empty()) {
			// poll_instructions += PollInstructions(kTrimRead);
		}
	}

	if (!msg_buffer_.IsFull()) {
		/*
		header NL
		header NL
		header NL
		NL
		message_body

		BL = NLNL
		kState = kParseHeader | kParseBody
		*/
		/*BLまでがヘッダ、それ以降はボディ
		ヘッダはNLでsplit、keyvalueに分解してheader_fields_に追加
		ボディはmessage_body_に代入
		*/
		ThinString buf;
		while (true) {
			Result<char> res = read_buffer_.GetChar();
			if (res.IsErr()) {
				return Error(res.Err());
			}
			char c = res.Val();
		}
	}
	// cgiレスポンスをhttpレスポンスに変換
	/* ヘッダは、ヘッダごとに固有の変換方法があるので、それに従う
	例えば、Statusヘッダは、httpレスポンスのstatus-codeに変換される
	ボディは何も変更せずにhttpメッセージのbodyとする
	*/
	// msg_buffer_.push_back();
	return poll_instructions;
}

int Cgi::StartCgiProcess(const char *file, char **argv, char **envp) const
{
	int pipe_from_cgi[2];
	ExpSafetyPipe(pipe_from_cgi);

	pid_t pid = fork();
	switch (pid) {
	case -1:
		throw std::runtime_error("fork: " + std::string(strerror(errno)));
	case 0:
		ExpSafetyDup2(pipe_to_cgi_[READ], STDIN_FILENO);
		ExpSafetyDup2(pipe_from_cgi[WRITE], STDOUT_FILENO);
		ExpSafetyClose(pipe_to_cgi_[READ]);
		ExpSafetyClose(pipe_from_cgi[WRITE]);
		ExpSafetyClose(pipe_to_cgi_[WRITE]);
		ExpSafetyClose(pipe_from_cgi[READ]);

		execve(file, argv, envp);
		throw std::runtime_error("execve: " + std::string(strerror(errno)));
	default:
		ExpSafetyClose(pipe_to_cgi_[READ]);
		ExpSafetyClose(pipe_from_cgi[WRITE]);
	}
	return pid;
}

void Cgi::ExpSafetyPipe(int *fds) const
{
	if (pipe(fds) < 0) {
		throw std::runtime_error("pipe: " + std::string(strerror(errno)));
	}
}

void Cgi::ExpSafetyClose(int fd) const
{
	if (close(fd) == -1) {
		throw std::runtime_error("close: " + std::string(strerror(errno)));
	}
}

void Cgi::ExpSafetyDup2(int oldfd, int newfd) const
{
	if (dup2(oldfd, newfd) == -1) {
		throw std::runtime_error("dup2: " + std::string(strerror(errno)));
	}
}

std::string Cgi::MakeKeyValueString(const std::string &key, const std::string &value)
{
	return key + "=" + value;
}
