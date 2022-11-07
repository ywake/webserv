#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <list>

#include "event.hpp"
namespace event
{
	typedef std::list<Instruction> Instructions;
	typedef struct Instruction {
		enum Command {
			kAppendEventType,
			kTrimEventType,
			kRegister,
			kUnregister
		};
		Command command;
		Event   event;
	} Instruction;
} // namespace event

#endif
