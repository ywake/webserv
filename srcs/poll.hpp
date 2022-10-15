#ifndef POLL_HPP
#define POLL_HPP

#include <vector>

#include "poll_instruction.hpp"
#include "result.hpp"

namespace io_multiplexer
{
	typedef std::vector<struct epoll_event> PollEvents;

	class Poll
	{
	  private:
		int epoll_fd_;
		int timeout_;
		int maxevents_;

	  public:
		Poll(int timeout = -1);
		Result<PollEvents> Wait();
	};
} // namespace io_multiplexer

#endif
