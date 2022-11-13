#ifndef CGI_REQUEST_HPP
#define CGI_REQUEST_HPP

#include "request_message.hpp"
#include <string>

namespace cgi
{
	//[FIX] cgi_request_builder
	class RequestMessage
	{
	  private:
		const http::RequestMessage message_;
		const RequestFormData     &formdata_;
		std::vector<std::string>   meta_variables_;
		std::string                script_path_;
		std::string                extra_path_;

	  public:
		RequestMessage(const http::RequestMessage &message);
		const http::RequestMessage     GetHttpRequest() const;
		const std::string              GetScriptPath() const;
		const std::vector<std::string> GetMetaVariables() const;
		void                           SetMetaVariables(
									  const std::string server_name,
									  const std::string server_port,
									  const std::string client_ip
								  );

	  private:
		void        SearchScriptPath();
		void        SetContentLength();
		void        SetContentType();
		void        SetGateWayInterFace();
		void        SetPathInfo();
		void        SetScriptName();
		void        SetQueryString();
		void        SetRemoteAddr(const std::string &remote_addr);
		void        SetRequestMethod();
		void        SetServerName(const std::string &server_name);
		void        SetServerPort(const std::string &server_port);
		void        SetServerProtocol();
		void        SetServerSoftWare();
		std::string MakeKeyValueString(const std::string &key, const std::string &value);
	};

} // namespace cgi

#endif // CGI_REQUEST_HPP
