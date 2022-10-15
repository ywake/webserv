#include "poll.hpp"

#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <sys/epoll.h>

namespace io_multiplexer
{
	Poll::Poll(int timeout) : timeout_(timeout), maxevents_()
	{
		epoll_fd_ = epoll_create1(EPOLL_CLOEXEC);
		if (epoll_fd_ == -1) {
			throw std::runtime_error(std::string("epoll_create: ") + strerror(errno));
		}
	}

	Result<PollEvents> Poll::Wait()
	{
		PollEvents          events(maxevents_);
		struct epoll_event *events_ptr  = events.data();
		int                 events_size = 0;

		while (true) {
			events_size = epoll_wait(epoll_fd_, events_ptr, maxevents_, timeout_);
			if (events_size != -1) {
				break;
			} else if (errno == EINTR)
				continue;
			else {
				return Error(std::string("epoll_wait: ") + strerror(errno));
			}
		}
		events.resize(events_size);
		return events;
	}


} // namespace io_multiplexer
