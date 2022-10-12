#ifndef SENDER_HPP
#define SENDER_HPP

#include "poll_instruction.hpp"
#include "request_message.hpp"
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
		http::RequestMessage     request_; // sender 側で変更し得るので非const
		conf::VirtualServerConf &config_;
		const SockAddrIn        &client_;

	  private:
		Sender();

	  public:
		Sender(
			int                            fd,
			const conf::VirtualServerConf &config,
			const http::RequestMessage    &request,
			const SockAddrIn              &client
		);
		io_multiplexer::PollInstruction Proceed();
		bool                            IsFinished();
	};
} // namespace server

#endif
