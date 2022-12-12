#include <cerrno>
#include <cstdio>
#include <sys/time.h>
#include <time.h>

#include "connection.hpp"
#include "debug.hpp"
namespace server
{
	using namespace event;

	const time_t                   Connection::kTimeoutDuration     = 60;
	const std::size_t              Connection::kMaxRecverBufSize    = 8196;
	const std::size_t              Connection::kMaxSenderBufSize    = 8196;
	const std::size_t              Connection::kMaxRequestQueueSize = 3;
	const conf::VirtualServerConfs Connection::kEmptyConfs          = conf::VirtualServerConfs();

	Connection::Connection()
		: Socket(),
		  configs_(kEmptyConfs),
		  client_(),
		  request_holder_(),
		  is_finished_(false),
		  time_()
	{}

	Connection::Connection(
		int fd, const conf::VirtualServerConfs &configs, const SockAddrStorage &client
	)
		: Socket(fd),
		  configs_(configs),
		  client_(client),
		  reciever_(fd),
		  request_holder_(&configs),
		  response_holder_(fd, configs, RequestHolder::DestroyRequest),
		  is_finished_(false),
		  time_()
	{
		clock_gettime(CLOCK_MONOTONIC_RAW, &time_);
	}

	// setからeraseするためだけの存在
	Connection::Connection(int fd)
		: Socket(fd),
		  configs_(kEmptyConfs),
		  client_(),
		  reciever_(-1),
		  request_holder_(),
		  is_finished_(false),
		  time_()
	{}

	// // Connection::Connection(const Connection &other)
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
		DBG_INFO;
		event::Instructions insts;

		clock_gettime(CLOCK_MONOTONIC_RAW, &time_);
		if (event_type & event::Event::kRead) {
			log("con recv");
			Instructions recv_insts = Recieve();
			insts.splice(insts.end(), recv_insts);
		}
		if (event_type & event::Event::kWrite) {
			log("con write");
			Instructions send_insts = Send();
			insts.splice(insts.end(), send_insts);
		}
		return insts;
	}

	Instructions Connection::OnEof()
	{
		Instructions insts;

		insts.push_back(Instruction(Instruction::kTrimEventType, GetFd(), Event::kRead));
		if (shutdown(GetFd(), SHUT_RD) < 0) {
			std::cerr << "shutdown" << strerror(errno) << std::endl;
			// TDDO 500
		}
		request_holder_.OnEof();
		if (CanStartNewTask()) { // TODO fix tmp とか
			Instructions tmp = StartResponse();
			insts.splice(insts.end(), tmp);
		}
		return insts;
	}

	Instructions Connection::Recieve()
	{
		Instructions insts;

		if (reciever_.IsEof() && reciever_.empty()) {
			log("con eof");
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
				// TDDO 500 専用のappend taskみたいのをholderに作る
			}
		}
		if (!is_holder_full) {
			request_holder_.Parse(reciever_);
		}
		if (CanStartNewTask()) { // TODO fix tmp とか
			Instructions tmp = StartResponse();
			insts.splice(insts.end(), tmp);
		}
		return insts;
	}

	Instructions Connection::Send()
	{
		Result<Instructions> res = response_holder_.Send();
		if (res.IsErr() || response_holder_.IsFatal()) {
			is_finished_     = true;
			Instructions tmp = response_holder_.UnregisterAll();
			res.Val().splice(res.Val().end(), tmp);
			return res.Val(); // TODO refactor;
		}
		if (CanStartNewTask()) {
			Instructions tmp = StartResponse();
			res.Val().splice(res.Val().end(), tmp);
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

	// TODO パイプラインは冪等かどうかの判定と、response holder内部の変更が必要
	bool Connection::CanStartNewTask()
	{
		return request_holder_.size() != 0 && response_holder_.size() == 0;
	}

	Instructions Connection::Disconnect()
	{
		log("disconnect", GetFd());
		Instructions insts = response_holder_.UnregisterAll();
		int          fd    = managed_fd_.GetFd();
		insts.push_back(Instruction(Instruction::kUnregister, fd));
		return insts;
	}

	bool Connection::IsTimeOut()
	{
		struct timespec now;
		if (clock_gettime(CLOCK_MONOTONIC_RAW, &now) == -1) {
			DBG_INFO;
			log("istimeout clock_gettime", strerror(errno));
		}
		log("time duration", now.tv_sec - time_.tv_sec);
		return now.tv_sec - time_.tv_sec >= kTimeoutDuration;
	}

} // namespace server
