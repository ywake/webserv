#include "cgi.hpp"
#include "webserv_utils.hpp"

#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>

Cgi::Cgi(
	const http::RequestMessage &message,
	const std::string           server_name,
	const std::string           server_port,
	const std::string           client_ip
)
	: message_(message),
	  formdata_(message.request_line_.request_target_.GetRequestFormData()),
	  server_name_(server_name),
	  server_port_(server_port),
	  client_ip_(client_ip)

{
	ExpSafetyPipe(pipe_to_cgi_);
}

// MUSTなメタ変数
// https://wiki.suikawiki.org/n/CGI%E3%83%A1%E3%82%BF%E5%A4%89%E6%95%B0#section-%E3%83%A1%E3%82%BF%E5%A4%89%E6%95%B0%E3%81%AE%E4%B8%80%E8%A6%A7
void Cgi::SetMetaVariables()
{
	SetGateWayInterFace();
	SetQueryString();
	SetRemoteAddr();
	SetRequestMethod();
	SetServerName();
	SetServerPort();
	SetServerProtocol();
	SetServerSoftWare();

	if (message_.HasMessageBody()) {
		SetContentLength();
	}
	if (message_.field_lines_.Contains("content-type")) {
		SetContentType();
	}

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

void Cgi::SetPathInfo(const std::string &value)
{
	meta_variables_.push_back(MakeKeyValueString("path-info", value));
}

void Cgi::SetScriptName(const std::string &value)
{
	meta_variables_.push_back(MakeKeyValueString("script-name", value));
}

void Cgi::SetQueryString()
{
	meta_variables_.push_back(MakeKeyValueString("query-string", formdata_.query_.ToString()));
}

void Cgi::SetRemoteAddr()
{
	meta_variables_.push_back(MakeKeyValueString("remote-addr", client_ip_));
}

void Cgi::SetRequestMethod()
{
	meta_variables_.push_back(
		MakeKeyValueString("request-method", message_.request_line_.method_.ToString())
	);
}

void Cgi::SetServerName()
{
	meta_variables_.push_back(MakeKeyValueString("server-name", server_name_));
}

void Cgi::SetServerPort()
{
	meta_variables_.push_back(MakeKeyValueString("server-port", server_port_));
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

std::string Cgi::MakeKeyValueString(const std::string &key, const std::string &value)
{
	return key + "=" + value;
}
