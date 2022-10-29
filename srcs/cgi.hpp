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
	http::RequestMessage     message_;
	std::vector<std::string> meta_variables_;
	int                      pipe_to_cgi[TYPE_SIZE];

  public:
	Cgi(const http::RequestMessage &message);
	// cgiレスポンスをhttpレスポンスに変換する
	int                   Cgi::Run();
	http::ResponseMessage Read() const;
	void                  SetMetaVariables();
	ssize_t               WriteRequestData(size_t nbyte) const;

  private:
	int  StartCgiProcess(const char *file, char **argv, char **envp);
	void SetContentLength();
	void ExpSafetyPipe(int *fds) const;
	void ExpSafetyClose(int fd) const;
	void ExpSafetyDup2(int oldfd, int newfd) const;
};

#endif // CGI_HPP
