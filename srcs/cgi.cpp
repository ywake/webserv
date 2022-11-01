#include "cgi.hpp"
#include "webserv_utils.hpp"

#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>

Cgi::Cgi(const http::RequestMessage &message)
	: message_(message), formdata_(message.request_line_.request_target_.GetRequestFormData())
{
	ExpSafetyPipe(pipe_to_cgi_);
}

// MUSTなメタ変数
// https://wiki.suikawiki.org/n/CGI%E3%83%A1%E3%82%BF%E5%A4%89%E6%95%B0#section-%E3%83%A1%E3%82%BF%E5%A4%89%E6%95%B0%E3%81%AE%E4%B8%80%E8%A6%A7
void Cgi::SetMetaVariables()
{
	if (message_.HasMessageBody()) {
		SetContentLength();
	}
	if (message_.field_lines_.Contains("content-type")) {
		SetContentType();
	}
	SetGateWayInterFace();

	std::vector<ThinString> hierarchy = Split(formdata_.path_, "/");

	std::string script_path;
	for (std::vector<ThinString>::iterator it = hierarchy.begin(); it != hierarchy.end(); it++) {
		std::string lower_path = it->ToString();
		script_path += lower_path;
		if (!IsRegularFile(script_path) || !EndsWith(lower_path, ".php")) {
			continue;
		}
		SetScriptName(script_path);

		std::string extra_path;
		for (std::vector<ThinString>::iterator i = ++it; i != hierarchy.end(); i++) {
			extra_path += it->ToString();
		}
		SetPathInfo(extra_path);
	}
}

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

void Cgi::SetPathInfo(const std::string &value)
{
	const std::string key = "path-info";
	meta_variables_.push_back(key + "=" + value);
}

void Cgi::SetScriptName(const std::string &value)
{
	const std::string key = "script-name";
	meta_variables_.push_back(key + "=" + value);
}

ssize_t Cgi::WriteRequestData(size_t nbyte) const
{
	return write(pipe_to_cgi_[WRITE], message_.message_body_.c_str(), nbyte);
}

int Cgi::Run()
{
	const char *file = formdata_.path_.ToString().c_str();
	char       *argv[script_cmdlines_.size() + 1];
	char       *envp[meta_variables_.size() + 1];

	for (std::size_t i = 0; i < script_cmdlines_.size(); i++) {
		argv[i] = const_cast<char *>(script_cmdlines_[i].c_str());
	}
	argv[script_cmdlines_.size()] = NULL;

	for (std::size_t i = 0; i < meta_variables_.size(); i++) {
		envp[i] = const_cast<char *>(meta_variables_[i].c_str());
	}
	envp[meta_variables_.size()] = NULL;

	return StartCgiProcess(file, argv, envp);
}

int Cgi::StartCgiProcess(const char *file, char **argv, char **envp)
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
