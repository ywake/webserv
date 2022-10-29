#ifndef CGI_HPP
#define CGI_HPP

#include "request_message.hpp"
#include "response_message.hpp"

class Cgi
{
  private:
	http::RequestMessage     message_;
	std::vector<std::string> meta_variables_;
	int                      pipe_to_cgi[2];
	enum PIPE_TYPE {
		READ,
		WRITE
	};

  public:
	// httpメッセージからcgiリクエストを作成し、cgiプロセスを起動する
	Cgi(const http::RequestMessage &message);
	// cgiレスポンスをhttpレスポンスに変換する
	http::ResponseMessage Read() const;

  private:
	void SetMetaVariables();
	void SetContentLength();
	ssize_t WriteRequestData(size_t nbyte) const;
	int  StartCgiProcess();
};

#endif // CGI_HPP
