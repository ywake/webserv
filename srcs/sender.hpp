#ifndef SENDER_HPP
#define SENDER_HPP

#include "poll_instruction.hpp"
#include "server_conf.hpp"

namespace server
{
	class Sender
	{
	  private:
		int              fd_;
		conf::ServerConf config;

	  public:
		io_multiplexer::PollInstruction Proceed();
	};
} // namespace server
#endif
