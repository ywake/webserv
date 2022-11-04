#ifndef CGI_HPP
#define CGI_HPP

#include "i_resource.hpp"
#include "request_message.hpp"
#include "response_message.hpp"
#include "result/result.hpp"

class Cgi : public http::IResource
{
  private:
	enum PIPE_TYPE {
		READ,
		WRITE,
		TYPE_SIZE
	};

	const http::RequestMessage message_;
	const RequestFormData      formdata_;

	std::string              script_path_;
	std::string              extra_path_;
	std::vector<std::string> meta_variables_;
	int                      pipe_to_cgi_[TYPE_SIZE];
	pid_t                    pid_;

  public:
	Cgi(const http::RequestMessage &message);
	void Cgi::Run(const std::string &cgi_path);
	bool Cgi::Terminate() const;
	void SetMetaVariables(
		const std::string server_name, const std::string server_port, const std::string client_ip
	);
	ssize_t                        WriteRequestData(size_t nbyte) const;
	std::vector<std::string>       GetMetaVariables() const;
	const Result<PollInstructions> Send();
	const Result<PollInstructions> Receive();

  private:
	int                       StartCgiProcess(const char *file, char **argv, char **envp) const;
	void                      SetContentLength();
	void                      SetContentType();
	void                      SetGateWayInterFace();
	void                      SetPathInfo();
	void                      SetScriptName();
	void                      SetQueryString();
	void                      SetRemoteAddr(const std::string &remote_addr);
	void                      SetRequestMethod();
	void                      SetServerName(const std::string &server_name);
	void                      SetServerPort(const std::string &server_port);
	void                      SetServerProtocol();
	void                      SetServerSoftWare();
	void                      ExpSafetyPipe(int *fds) const;
	void                      ExpSafetyClose(int fd) const;
	void                      ExpSafetyDup2(int oldfd, int newfd) const;
	std::string               MakeKeyValueString(const std::string &key, const std::string &value);
	void                      SearchScriptPath();
	Result<std::vector<char>> Cgi::Read(size_t nbyte) const;
};

class CgiResponse
{
};

#endif // CGI_HPP
