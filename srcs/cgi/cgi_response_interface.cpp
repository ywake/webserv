#include "cgi_response.hpp"

#include "debug.hpp"
#include "http_exceptions.hpp"
#include <sys/socket.h>

namespace cgi
{
	/**
	 * IResponse
	 */
	void CgiResponse::Perform(const event::Event &event)
	{
		// if (event.event_type & event::Event::kHangUp) {
		// 	log("cgi hangup");
		// 	parent_fd_.Close();
		// }
		if (event.event_type & event::Event::kWrite) {
			log("cgi write ready");
			OnWriteReady();
		}
		if (event.event_type & event::Event::kRead) {
			log("cgi read ready");
			OnReadReady();
		}
	}

	void CgiResponse::OnWriteReady()
	{
		if (parent_fd_.GetFd() == ManagedFd::kNofd) {
			log("nofd");
			return;
		}
		if (body_writer_.IsFinished()) {
			log("write to cgi is finished");
			if (shutdown(parent_fd_.GetFd(), SHUT_WR) < 0) {
				log("shutdown failed");
				throw http::InternalServerErrorException();
			}
			return;
		}
		log("write body to cgi");
		if (body_writer_.Write(parent_fd_.GetFd()).IsErr()) {
			shutdown(parent_fd_.GetFd(), SHUT_WR);
		}
	}

	void CgiResponse::OnReadReady()
	{
		if (parent_fd_.GetFd() == ManagedFd::kNofd || is_finished_) {
			return;
		}
		log("read from cgi");
		int read_size = parser_.Read(parent_fd_.GetFd());
		if (read_size < 0) {
			throw http::InternalServerErrorException();
		}
		if (parser_.IsFinished()) {
			push_back(parser_.ToString());
		}
	}

	Result<void> CgiResponse::Send(int fd)
	{
		return Write(fd);
	}

	bool CgiResponse::HasReadyData() const
	{
		return !QueuingBuffer::empty();
	}

	bool CgiResponse::HasFd() const
	{
		return parent_fd_.GetFd() != ManagedFd::kNofd;
	}

	Emptiable<int> CgiResponse::GetFd() const
	{
		return parent_fd_.GetFd();
	}

	std::size_t CgiResponse::size() const
	{
		return q_buffer::QueuingBuffer::size();
	}

	bool CgiResponse::IsFinished() const
	{
		return parser_.IsFinished();
	}
} // namespace cgi
