#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <list>

#include "event.hpp"

// TODO 名前空間変えた方がいいかも
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

	typedef std::list<Instruction> Instructions;
} // namespace event

#endif
