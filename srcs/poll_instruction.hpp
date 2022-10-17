#ifndef POLL_INSTRUCTION_HPP
#define POLL_INSTRUCTION_HPP

#include <list>
#include <sys/epoll.h>

namespace io_multiplexer
{
	typedef struct PollInstruction                     PollInstruction;
	typedef std::list<io_multiplexer::PollInstruction> PollInstructions;

	struct PollInstruction {
	  public:
		enum Command {
			kAppendEventType,
			kTrimEventType,
			kRegister,
			kUnregister
		};

	  public:
		int                fd;
		struct epoll_event event;
		Command            command;
	};
} // namespace io_multiplexer

#endif
