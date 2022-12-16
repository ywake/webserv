#include <cerrno>
#include <fcntl.h>

#include "debug.hpp"
#include "http_define.hpp"
#include "http_exceptions.hpp"
#include "static_response.hpp"
#include "webserv_utils.hpp"

namespace server
{
	StaticResponse::StaticResponse(const IRequest &request, const conf::LocationConf &conf)
		: request_(request), config_(conf), is_finished_(false)
	{
		log("static res construct");
		if (request_.Method() == http::methods::kGet) {
			InitGetMethod();
		} else {
			throw http::NotImplementedException();
		}
	}

	void StaticResponse::InitGetMethod()
	{
		const std::string &root = config_.GetRoot();
		const std::string  path = utils::JoinPath(root, request_.Path());

		int fd = open(path.c_str(), O_RDONLY | O_NONBLOCK | O_CLOEXEC);
		if (fd == -1) {
			switch (errno) {
			case EACCES:
				throw http::ForbiddenException();
			case ENOENT:
			case ENAMETOOLONG:
				throw http::NotFoundException();
			default:
				throw http::InternalServerErrorException();
			}
		}
		managed_fd_ = fd;
	}

	void StaticResponse::Perform(const event::Event &event)
	{
		(void)event;
		ssize_t read_size = Read(managed_fd_.GetFd());
		if (read_size < 0) {
			throw http::InternalServerErrorException();
		}
		is_finished_ = read_size == 0;
	}

	Result<void> StaticResponse::Send(int fd)
	{
		return Write(fd);
	}

	bool StaticResponse::HasReadyData() const
	{
		return !QueuingBuffer::empty();
	}

	bool StaticResponse::HasFd() const
	{
		return managed_fd_.GetFd() != ManagedFd::kNofd;
	}

	Emptiable<int> StaticResponse::GetFd() const
	{
		return managed_fd_.GetFd();
	}

	// TODO eofとか
	bool StaticResponse::IsFinished() const
	{
		return is_finished_;
	}

	std::size_t StaticResponse::size() const
	{
		return q_buffer::QueuingBuffer::size();
	}

} // namespace server
