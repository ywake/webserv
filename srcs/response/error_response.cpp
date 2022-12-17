#include <cerrno>
#include <fcntl.h>

#include "debug.hpp"
#include "error_response.hpp"
#include "http_define.hpp"
#include "http_exceptions.hpp"

namespace response
{
	ErrorResponse::ErrorResponse(
		const server::IRequest &request,
		const http::StatusCode &status_code,
		const conf::ServerConf &conf
	)
		: AResponse(request), config_(conf)
	{
		log("error res construct", status_code.GetCode());
		MetaDataStorage::StoreStatusLine(http::kHttpVersion, status_code);
		MetaDataStorage::StoreHeader("Server", http::kServerName);
		// MetaDataStorage::StoreHeader("Connection", "keep-alive"); request情報必要
		// content-length;
		MetaDataStorage::PushWithCrLf();
		PushDefaultErrorPage(status_code);
		is_finished_ = true;
	}

	void ErrorResponse::PushDefaultErrorPage(const http::StatusCode &code)
	{
		std::string err_msg  = code.GetCodeStr() + " " + code.GetReasonPhrase();
		std::string err_page = "<html>\n"
							   "<head><title>" +
							   err_msg +
							   "</title></head>\n"
							   "<body>\n"
							   "<center><h1>" +
							   err_msg +
							   "</h1></center>\n"
							   "<hr><center>webserv/1.0</center>\n"
							   "</body>\n"
							   "</html>\n";
		push_back(err_page);
	}

	void ErrorResponse::Perform(const event::Event &event)
	{
		(void)event;
	}

	bool ErrorResponse::HasFd() const
	{
		return managed_fd_.GetFd() != ManagedFd::kNofd;
	}

	Emptiable<int> ErrorResponse::GetFd() const
	{
		return managed_fd_.GetFd();
	}

} // namespace response
