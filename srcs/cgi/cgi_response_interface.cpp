#include "cgi_response.hpp"

#include "http_exceptions.hpp"

namespace cgi
{
	/**
	 * IResponse
	 */
	void CgiResponse::Perform(const event::Event &event)
	{
		if (event.event_type & event::Event::kWrite) {
			OnWriteReady();
		}
		if (event.event_type & event::Event::kRead) {
			OnReadReady();
		}
	}

	void CgiResponse::OnWriteReady()
	{
		if (parent_fd_.GetFd() != ManagedFd::kNofd && !body_writer_.IsFinished()) {
			body_writer_.Write(parent_fd_.GetFd());
		}
	}

	void CgiResponse::ExecCgi()
	{
		pid_t pid = fork();
		switch (pid) {
		case -1:
			throw http::InternalServerErrorException();
			break;
		case 0:
			ChildProcess();
			break;
		default:
			ParentProcess(pid);
			break;
		}
	}

	void CgiResponse::ParentProcess(pid_t pid)
	{
		(void)pid;
		child_fd_.Close();
	}

	void CgiResponse::OnReadReady() {}

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
		return is_finished_;
	}
} // namespace cgi
