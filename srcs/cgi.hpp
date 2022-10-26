#ifndef CGI_HPP
#define CGI_HPP

#include "http_message.hpp"

class Cgi
{
  private:
	HttpMessage            message_;
	std::list<std::string> meta_variables_;

  public:
	// httpメッセージからcgiリクエストを作成し、cgiプロセスを起動する
	Cgi(const HttpMessage &message);
	// cgiレスポンスをhttpレスポンスに変換する
	HttpMessage Read() const;

  private:
	void        SetMetaVariables();
	void        PutMetaVariables() const;
	std::string RebuildHeader(const std::string &name) const;
	void        SetContentLength();
};

#endif // CGI_HPP
