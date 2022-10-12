#ifndef SENDER_HPP
#define SENDER_HPP

#include "poll_instruction.hpp"
#include "virtual_server_conf.hpp"

namespace server
{
	class Sender
	{
	  private:
		enum State {
			kReading,
			kSending,
			kFinished,
		};

	  private:
		int                      fd_;
		State                    state_;
		conf::VirtualServerConf &config_;

	  public:
		io_multiplexer::PollInstruction Proceed();
	};
} // namespace server
#endif
