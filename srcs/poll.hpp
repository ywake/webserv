#ifndef POLL_HPP
#define POLL_HPP

#include <list>
#include <map>
#include <vector>

#include "poll_instruction.hpp"
#include "result.hpp"

namespace io_multiplexer
{
	typedef std::vector<struct epoll_event>           PollEvents;
	typedef std::list<Result<const PollInstruction> > ErrEvents;

	class Poll
	{
	  private:
		int                               epoll_fd_;
		int                               timeout_;
		std::map<int, struct epoll_event> registered_events_;

	  public:
		Poll(int timeout = -1);
		Result<PollEvents> Wait();
		ErrEvents          InstructMulti(const PollInstructions &instructions);
		Result<void>       Instruct(const PollInstruction &instruction);
		Result<void>       AppendEventType(const PollInstruction &instruction);
		Result<void>       TrimEventType(const PollInstruction &instruction);
		Result<void>       Register(int fd, const struct epoll_event &event);
		Result<void>       Unregister(int fd);
	};
} // namespace io_multiplexer

#endif
