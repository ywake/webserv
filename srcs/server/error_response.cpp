#include <cerrno>
#include <fcntl.h>

#include "debug.hpp"
#include "error_response.hpp"
#include "http_define.hpp"
#include "http_exceptions.hpp"

namespace server
{
	ErrorResponse::ErrorResponse(const http::StatusCode &status_code, const conf::ServerConf &conf)
		: config_(conf), is_finished_(false)
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

	Result<void> ErrorResponse::Send(int fd)
	{
		return Write(fd);
	}

	bool ErrorResponse::HasReadyData() const
	{
		return !QueuingBuffer::empty();
	}

	bool ErrorResponse::HasFd() const
	{
		return managed_fd_.GetFd() != ManagedFd::kNofd;
	}

	Emptiable<int> ErrorResponse::GetFd() const
	{
		return managed_fd_.GetFd();
	}

	bool ErrorResponse::IsFinished() const
	{
		return is_finished_;
	}

	std::size_t ErrorResponse::size() const
	{
		return q_buffer::QueuingBuffer::size();
	}

} // namespace server
