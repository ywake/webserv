#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "poll_instruction.hpp"
#include "receiver.hpp"
#include "sender.hpp"
#include "server_types.hpp"
#include "virtual_server_confs.hpp"

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
		int                             fd_;
		const conf::VirtualServerConfs &configs_;
		const SockAddrIn                client_;
		State                           state_;
		Receiver                        receiver_;
		Sender                         *sender_;

	  public:
		Connection(int fd, const conf::VirtualServerConfs &conf, const SockAddrIn &client);
		~Connection();
		PollInstructions Proceed();
		bool             IsFinished();

	  private:
		PollInstructions Receive();
		PollInstructions Send();
		PollInstructions Open();
	};
} // namespace server
#endif
