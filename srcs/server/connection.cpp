#include <cerrno>
#include <cstdio>

#include "connection.hpp"

namespace server
{
	using namespace event;

	const std::size_t              Connection::kMaxRecverBufSize    = 8196;
	const std::size_t              Connection::kMaxSenderBufSize    = 8196;
	const std::size_t              Connection::kMaxRequestQueueSize = 3;
	const conf::VirtualServerConfs Connection::kEmptyConfs          = conf::VirtualServerConfs();

	Connection::Connection() : Socket(), configs_(kEmptyConfs), client_(), request_holder_() {}

	Connection::Connection(
		int fd, const conf::VirtualServerConfs &configs, const SockAddrStorage &client
	)
		: Socket(fd), configs_(configs), client_(client), reciever_(fd), request_holder_()
	{}

	Connection::Connection(const Connection &other)
		: Socket(other), configs_(other.configs_), client_(other.client_)
	{}

	bool Connection::operator<(const Connection &other) const
	{
		return this->GetFd() < other.GetFd();
	}

	Connection::~Connection() {}

	Instructions Connection::Proceed(const Event &event)
	{
		if (event.fd == this->GetFd()) {
			return CommunicateWithClient(event.event_type);
		} else {
		}
		return Instructions();
	}

	Instructions Connection::CommunicateWithClient(uint32_t event_type)
	{
		event::Instructions insts;

		if (event_type & event::Event::kRead) {
			Instructions recv_insts = Recieve();
			insts.splice(insts.end(), recv_insts);
		}
		if (event_type & event::Event::kWrite) {
			Instructions send_insts = Send();
			insts.splice(insts.end(), send_insts);
		}
		return insts;
	}

	Instructions Connection::OnEof()
	{
		// TODO 最初の時用にリソース処理追加

		Instructions insts;

		insts.push_back(Instruction(Instruction::kTrimEventType, GetFd(), Event::kRead));
		if (shutdown(GetFd(), SHUT_RD) < 0) {
			std::cerr << "shutdown" << strerror(errno) << std::endl;
			// TDDO 500
		}
		request_holder_.OnEof();
		// if (CanResPonsStart0) {}
		return insts;
	}

	Instructions Connection::Recieve()
	{
		Instructions insts;

		if (reciever_.IsEof() && reciever_.empty()) {
			return OnEof();
		}
		bool is_reciever_full = reciever_.size() >= kMaxRecverBufSize;
		bool is_holder_full   = request_holder_.size() >= kMaxRequestQueueSize;
		if (is_reciever_full && is_holder_full) {
			insts.push_back(Instruction(Instruction::kTrimEventType, GetFd(), Event::kRead));
			return insts;
		}
		if (!is_reciever_full) {
			Result<void> res = reciever_.Recv();
			if (res.IsErr()) {
				std::cerr << res.Err() << std::endl;
				// TDDO 500
			}
		}
		if (!is_holder_full) {
			request_holder_.Parse(reciever_);
		}
		// if (CanResPonsStart0) {}
		return Instructions();
	}

	Instructions Connection::Send()
	{
		return Instructions();
	}

} // namespace server
