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
			kRecv,
			kSend,
		};

	  private:
		typedef struct sockaddr_in SockAddrIn;

	  private:
		int                     fd_;
		const conf::ServerConf &configs_;
		const SockAddrIn        client_;
		State                   state_;
		Receiver                receiver_;
		Sender				 *sender;

	  public:
		Connection(int fd, const conf::ServerConf &conf, const SockAddrIn &client);
		State                           GetState();
		io_multiplexer::PollInstruction Proceed();

	  private:
		void                            Receive();
		io_multiplexer::PollInstruction Send();
	};
} // namespace server
#endif
