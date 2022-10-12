#ifndef SENDER_HPP
#define SENDER_HPP

#include "poll_instruction.hpp"
#include "virtual_server_conf.hpp"

namespace server
{
	class Sender
	{
	  private:
		int                     fd_;
		conf::VirtualServerConf config;

	  public:
		io_multiplexer::PollInstruction Proceed();
	};
} // namespace server
#endif
