#include "event.hpp"
#include "instruction.hpp"

std::ostream &operator<<(std::ostream &os, const event::Event &event)
{
	os << "fd: " << event.fd << std::endl;
	os << "type: " << event.event_type << std::endl;
	if (event.event_type & event::Event::kWrite) {
		os << "Write ";
	}
	if (event.event_type & event::Event::kRead) {
		os << "Read ";
	}
	if (event.event_type == event::Event::kEmpty) {
		os << "Empty ";
	}
	os << std::endl;
	return os;
}

std::ostream &operator<<(std::ostream &os, const event::Instruction &instruction)
{
	switch (instruction.command) {
	case event::Instruction::kAppendEventType:
		os << "AppendEventType" << std::endl;
		break;
	case event::Instruction::kTrimEventType:
		os << "TrimEventType" << std::endl;
		break;
	case event::Instruction::kRegister:
		os << "Register" << std::endl;
		break;
	case event::Instruction::kUnregister:
		os << "Unregister" << std::endl;
		break;
	}
	os << instruction.event << std::endl;
	return os;
}
