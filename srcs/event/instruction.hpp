#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <list>

#include "event.hpp"

// TODO 名前空間変えた方がいいかも
namespace event
{
	class Instruction
	{
	  public:
		enum Command {
			kAppendEventType,
			kTrimEventType,
			kRegister,
			kUnregister
		};
		Command command;
		Event   event;
		Instruction(
			Command  command    = kAppendEventType,
			int      fd         = -1,
			uint32_t event_type = Event::kEmpty,
			void    *data       = NULL
		);
	};

	typedef std::list<Instruction> Instructions;
} // namespace event

std::ostream &operator<<(std::ostream &os, const event::Instruction &instruction);

#endif
