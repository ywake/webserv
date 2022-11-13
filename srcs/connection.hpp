#ifndef CONNECTION_HPP
#define CONNECTION_HPP

// #include "poll_instruction.hpp"
// #include "receiver.hpp"
// #include "sender.hpp"
// #include "server_types.hpp"
#include "config/virtual_server_confs.hpp"
#include "managed_fd.hpp"

#include "instruction.hpp"
#include <netinet/in.h>
#include <sys/socket.h>

// TODO fd
namespace server
{
	typedef struct ::sockaddr_in SockAddrIn;

	class Connection
	{
		//   public:
		// 	enum State {
		// 		kReceiving,
		// 		kSending,
		// 		kFinished,
		// 	};

	  private:
		static const conf::VirtualServerConfs kEmptyConfs;

	  private:
		ManagedFd                       managed_fd_;
		const conf::VirtualServerConfs &configs_;
		const SockAddrIn                client_;
		// State                           state_;
		// Receiver                        receiver_;
		// Sender						 *sender_;

	  public:
		Connection();
		Connection(
			ManagedFd managed_fd, const conf::VirtualServerConfs &conf, const SockAddrIn &client
		);
		Connection(const Connection &other);
		~Connection();

		event::Instructions Proceed();
		// bool             IsFinished();

		//   private:
		// 	PollInstructions Receive();
		// 	PollInstructions Send();
	};
} // namespace server
#endif
