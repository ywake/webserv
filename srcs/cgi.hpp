#ifndef CGI_HPP
#define CGI_HPP

#include "request_message.hpp"
#include "response_message.hpp"

class Cgi
{
  private:
	enum PIPE_TYPE {
		READ,
		WRITE,
		TYPE_SIZE
	};

	const http::RequestMessage message_;
	const RequestFormData      formdata_;
	const std::string          server_name_;
	const std::string          server_port_;
	const std::string          client_ip_;

	std::vector<std::string> meta_variables_;
	std::vector<std::string> script_cmdlines_;
	int                      pipe_to_cgi_[TYPE_SIZE];

  public:
	Cgi(const http::RequestMessage &message,
		const std::string           server_name,
		const std::string           server_port,
		const std::string           client_ip);
	// cgiレスポンスをhttpレスポンスに変換する
	int                   Cgi::Run();
	http::ResponseMessage Read() const;
	void                  SetMetaVariables();
	ssize_t               WriteRequestData(size_t nbyte) const;

  private:
	int         StartCgiProcess(const char *file, char **argv, char **envp);
	void        SetContentLength();
	void        SetContentType();
	void        SetGateWayInterFace();
	void        SetPathInfo(const std::string &value);
	void        SetScriptName(const std::string &value);
	void        SetQueryString();
	void        SetRemoteAddr();
	void        SetRequestMethod();
	void        SetServerName();
	void        SetServerPort();
	void        SetServerProtocol();
	void        SetServerSoftWare();
	void        ExpSafetyPipe(int *fds) const;
	void        ExpSafetyClose(int fd) const;
	void        ExpSafetyDup2(int oldfd, int newfd) const;
	std::string MakeKeyValueString(const std::string &key, const std::string &value);
};

#endif // CGI_HPP
