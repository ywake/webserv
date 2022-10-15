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
		enum Instruction {
			kRegister,
			kUnregister,
		};

	  public:
		int         fd;
		uint32_t    event;
		Instruction instruction;
	};
} // namespace io_multiplexer

#endif
