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
		: Socket(fd),
		  configs_(configs),
		  client_(client),
		  reciever_(fd),
		  request_holder_(),
		  response_holder_(fd, configs, RequestHolder::DestroyRequest)
	{}

	// Connection::Connection(const Connection &other)
	// 	: Socket(other),
	// 	  configs_(other.configs_),
	// 	  client_(other.client_),
	// 	  request_holder_(other.request_holder_),
	// 	  response_holder_(-1, kEmptyConfs, RequestHolder::DestroyRequest) // tmp
	// {}

	bool Connection::operator<(const Connection &other) const
	{
		return this->GetFd() < other.GetFd();
	}

	Connection::~Connection() {}

	void Connection::SetConnectionPtr(Instructions &insts)
	{
		for (Instructions::iterator it = insts.begin(); it != insts.end(); ++it) {
			it->event.data = this;
		}
	}

	Instructions Connection::Proceed(const Event &event)
	{
		Instructions insts;

		if (event.fd == this->GetFd()) {
			insts = CommunicateWithClient(event.event_type);
		} else {
			insts = response_holder_.Perform(event);
		}
		SetConnectionPtr(insts);
		return insts;
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
		if (response_holder_.size() == 0) { // TODO fix tmp とか
			Instructions tmp = StartResponse();
			insts.splice(insts.end(), tmp);
		}
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
		if (response_holder_.size() == 0) { // TODO fix tmp とか
			Instructions tmp = StartResponse();
			insts.splice(insts.end(), tmp);
		}
		return Instructions();
	}

	Instructions Connection::Send()
	{
		Result<Instructions> res = response_holder_.Send();
		if (res.IsErr() || response_holder_.IsFatal()) {
			is_finished_ = true;
			return response_holder_.UnregisterAll();
		}
		return res.Val();
	}

	event::Instructions Connection::StartResponse()
	{
		Instructions insts;

		Emptiable<IRequest *> req = request_holder_.PopFront();
		if (!req.empty()) {
			insts = response_holder_.StartNewResponse(req.Value());
		}
		return insts;
	}

	bool Connection::IsFinished()
	{
		return is_finished_;
	}

} // namespace server
