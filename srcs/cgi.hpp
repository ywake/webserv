#ifndef CGI_HPP
#define CGI_HPP

#include "request_message.hpp"
#include "response_message.hpp"

class Cgi
{
  private:
	http::RequestMessage     message_;
	std::vector<std::string> meta_variables_;

  public:
	// httpメッセージからcgiリクエストを作成し、cgiプロセスを起動する
	Cgi(const http::RequestMessage &message);
	// cgiレスポンスをhttpレスポンスに変換する
	http::ResponseMessage Read() const;

  private:
	void        SetMetaVariables();
	std::string RebuildHeader(const std::string &name) const;
	void        SetContentLength();
	int         StartCgiProcess();
};

#endif // CGI_HPP
