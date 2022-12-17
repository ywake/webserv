#include "debug.hpp"
#include "event.hpp"
#include "instruction.hpp"

std::ostream &operator<<(std::ostream &os, const event::Event &event)
{
	os << "fd  : " << event.fd << std::endl;
	os << "type: " << event.event_type << " ";
	if (event.event_type == event::Event::kEmpty) {
		os << "Empty ";
	}
	if (event.event_type & event::Event::kWrite) {
		os << "Write ";
	}
	if (event.event_type & event::Event::kRead) {
		os << "Read ";
	}
	if (event.event_type & event::Event::kHangUp) {
		os << "HangUp ";
	}
	if (event.event_type & event::Event::kError) {
		os << "Error ";
	}
	os << std::endl;
	os << "ptr : " << event.data << std::endl;
	return os;
}

std::ostream &operator<<(std::ostream &os, const event::Instruction &instruction)
{
	switch (instruction.command) {
	case event::Instruction::kAppendEventType:
		os << COL_YELLOW "AppendEventType" COL_END << std::endl;
		break;
	case event::Instruction::kTrimEventType:
		os << COL_GREEN "TrimEventType" COL_END << std::endl;
		break;
	case event::Instruction::kRegister:
		os << COL_RED "Register" COL_END << std::endl;
		break;
	case event::Instruction::kUnregister:
		os << COL_BLUE "Unregister" COL_END << std::endl;
		break;
	}
	os << instruction.event << std::endl;
	return os;
}
