#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <netinet/in.h>

#include "poll_instruction.hpp"
#include "receiver.hpp"
#include "sender.hpp"
#include "server_conf.hpp"
#include "server_types.hpp"

// TODO fd
namespace server
{
	class Connection
	{
	  public:
		enum State {
			kReceiving,
			kSending,
			kFinished,
		};

	  private:
		typedef struct sockaddr_in SockAddrIn;

	  private:
		int                     fd_;
		const conf::ServerConf &configs_;
		const SockAddrIn        client_;
		State                   state_;
		Receiver                receiver_;
		Sender                  sender_;

	  public:
		Connection(int fd, const conf::ServerConf &conf, const SockAddrIn &client);
		PollInstructions Proceed();
		bool             IsFinished();

	  private:
		PollInstructions Receive();
		PollInstructions Send();
	};
} // namespace server
#endif
