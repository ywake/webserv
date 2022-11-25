#include "instruction.hpp"

namespace event
{
	Instruction::Instruction(Command command, int fd, uint32_t event_type, void *data)
		: command(command)
	{
		event.fd         = fd;
		event.event_type = event_type;
		event.data       = data;
	}
} // namespace event
