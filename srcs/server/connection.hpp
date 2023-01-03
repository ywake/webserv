#ifndef CONNECTION_HPP
#define CONNECTION_HPP

// #include "poll_instruction.hpp"
// #include "receiver.hpp"
// #include "sender.hpp"
// #include "server_types.hpp"
#include <deque>
#include <netinet/in.h>
#include <stdint.h>
#include <sys/socket.h>

#include "config/virtual_server_confs.hpp"
#include "instruction.hpp"
#include "managed_fd.hpp"
#include "receiver.hpp"
#include "request_holder.hpp"
#include "request_message.hpp"
#include "response_holder.hpp"
#include "socket.hpp"
#include "status_code.hpp"

// TODO fd
namespace server
{
	class Connection : public Socket
	{
	  public:
		static const time_t      kTimeoutDuration;
		static const std::size_t kMaxRequestQueueSize;

	  private:
		const conf::VirtualServerConfs &configs_;
		const SockAddrStorage           client_;
		Reciever                        receiver_;
		RequestHolder                   request_holder_;
		ResponseHolder                  response_holder_;
		bool                            is_finished_;
		struct ::timespec               time_;
		Connection();
		Connection(const Connection &other);
		Connection &operator=(const Connection &rhs);

	  public:
		Connection(
			int                             managed_fd,
			const conf::VirtualServerConfs &conf,
			const SockAddrStorage          &addr,
			const SockAddrStorage          &client
		);
		~Connection();
		bool                operator<(const Connection &other) const;
		event::Instructions CommunicateWithClient(uint32_t event_type);
		event::Instructions Proceed(const event::Event &event);
		bool                IsFinished();
		bool                IsTimeOut();
		event::Instructions Disconnect();

	  private:
		event::Instructions Recieve();
		event::Instructions Send();
		event::Instructions OnEof();
		event::Instructions StartResponse();
		bool                CanStartNewTask();
		void                SetConnectionPtr(event::Instructions &insts);
	};
} // namespace server
#endif
