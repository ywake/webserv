#ifndef CONNECTION_HPP
#define CONNECTION_HPP

// #include "poll_instruction.hpp"
// #include "receiver.hpp"
// #include "sender.hpp"
// #include "server_types.hpp"
#include <netinet/in.h>
#include <stdint.h>
#include <sys/socket.h>

#include "config/virtual_server_confs.hpp"
#include "instruction.hpp"
#include "managed_fd.hpp"
#include "reciever.hpp"
#include "socket.hpp"

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
	  public:
	  public:
		enum ParserState {
			kStatus,
			kHeader,
			kBody
		};
		static const std::size_t kMaxRecverBufSize;
		static const std::size_t kMaxSenderBufSize;

	  private:
		static const conf::VirtualServerConfs kEmptyConfs;

	  private:
		const conf::VirtualServerConfs &configs_;
		const SockAddrStorage           client_;
		Reciever                        reciever_;
		ParserState                     parser_state_;
		// State                           state_;
		// Sender						 *sender_;

	  public:
		Connection();
		Connection(
			int managed_fd, const conf::VirtualServerConfs &conf, const SockAddrStorage &client
		);
		Connection(const Connection &other);
		~Connection();
		bool                operator<(const Connection &other) const;
		event::Instructions CommunicateWithClient(uint32_t event_type);

		event::Instructions Proceed(const event::Event &event);
		// bool             IsFinished();

	  private:
		event::Instructions Recieve();
		event::Instructions Send();
		event::Instructions ParseRequest();
	};
} // namespace server
#endif
