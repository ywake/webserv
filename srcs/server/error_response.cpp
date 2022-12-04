#include <cerrno>
#include <fcntl.h>

#include "error_response.hpp"
#include "http_define.hpp"
#include "http_exceptions.hpp"

namespace server
{
	ErrorResponse::ErrorResponse(
		const http::StatusCode &status_code, const conf::LocationConf &conf
	)
		: config_(conf), is_finished_(false)
	{
		push_back(status_code.GetReasonPhrase());
		is_finished_ = true;
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
