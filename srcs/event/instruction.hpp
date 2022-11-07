#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include "event.hpp"
namespace event
{
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
