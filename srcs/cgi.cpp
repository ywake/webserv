#include "cgi.hpp"
#include "webserv_utils.hpp"

#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>

Cgi::Cgi(const http::RequestMessage &message) : message_(message)
{
	ExpSafetyPipe(pipe_to_cgi);
}

// MUSTなメタ変数
// https://wiki.suikawiki.org/n/CGI%E3%83%A1%E3%82%BF%E5%A4%89%E6%95%B0#section-%E3%83%A1%E3%82%BF%E5%A4%89%E6%95%B0%E3%81%AE%E4%B8%80%E8%A6%A7
void Cgi::SetMetaVariables()
{
	if (message_.HasMessageBody()) {
		SetContentLength();
	}
	// MUSTなメタ変数を設定していく
}

//メタ変数の構築の仕方はそれぞれ異なるかもしれないので、Set関数を作って最後にまとめてputenvする
void Cgi::SetContentLength()
{
	meta_variables_.push_back(message_.field_lines_.GetKeyValueString("content-length"));
}

ssize_t Cgi::WriteRequestData(size_t nbyte) const
{
	return write(pipe_to_cgi[WRITE], message_.message_body_.c_str(), nbyte);
}

int Cgi::StartCgiProcess()
{
	int pipe_from_cgi[2];
	ExpSafetyPipe(pipe_from_cgi);

	pid_t pid = fork();
	switch (pid) {
	case -1:
		throw std::runtime_error("fork: " + std::string(strerror(errno)));
	case 0:
		ExpSafetyDup2(pipe_to_cgi[READ], STDIN_FILENO);
		ExpSafetyDup2(pipe_from_cgi[WRITE], STDOUT_FILENO);
		ExpSafetyClose(pipe_to_cgi[READ]);
		ExpSafetyClose(pipe_from_cgi[WRITE]);
		ExpSafetyClose(pipe_to_cgi[WRITE]);
		ExpSafetyClose(pipe_from_cgi[READ]);

		ExecuteCgiScript();
	default:
		ExpSafetyClose(pipe_to_cgi[READ]);
		ExpSafetyClose(pipe_from_cgi[WRITE]);
	}
	return pid;
}

void Cgi::ExecuteCgiScript() const
{
	RequestFormData data = message_.request_line_.request_target_.GetRequestFormData();
	const char     *file = data.path_.ToString().c_str();
	char		   *argv[2];
	char		   *envp[meta_variables_.size() + 1];

	//[TODO]queryの解析と、search-wordでのパース
	// argv[search_word.size()]
	// for () {
	//	argv[N] = search_word[i];
	// }

	argv[0] = const_cast<char *>(data.query_.ToString().c_str());
	argv[1] = NULL;

	for (std::size_t i = 0; i < meta_variables_.size(); i++) {
		envp[i] = const_cast<char *>(meta_variables_[i].c_str());
	}
	envp[meta_variables_.size()] = NULL;

	execve(file, argv, envp);

	throw std::runtime_error("execve: " + std::string(strerror(errno)));
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
