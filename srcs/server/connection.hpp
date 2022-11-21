#ifndef CONNECTION_HPP
#define CONNECTION_HPP

// #include "poll_instruction.hpp"
// #include "receiver.hpp"
// #include "sender.hpp"
// #include "server_types.hpp"
#include "config/virtual_server_confs.hpp"
#include "managed_fd.hpp"
#include "socket.hpp"

#include "instruction.hpp"
#include <netinet/in.h>
#include <sys/socket.h>

// TODO fd
namespace server
{
	typedef struct sockaddr_storage SockAddrStorage;
	class Connection : public Socket
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
		const conf::VirtualServerConfs &configs_;
		const SockAddrStorage           client_;
		// State                           state_;
		// Receiver                        receiver_;
		// Sender						 *sender_;

	  public:
		Connection();
		Connection(
			ManagedFd                       managed_fd,
			const conf::VirtualServerConfs &conf,
			const SockAddrStorage          &client
		);
		Connection(const Connection &other);
		~Connection();
		bool operator<(const Connection &other) const;

		event::Instructions Proceed();
		// bool             IsFinished();

		//   private:
		// 	PollInstructions Receive();
		// 	PollInstructions Send();
	};
} // namespace server
#endif
