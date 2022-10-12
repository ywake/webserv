#include "connection.hpp"
#include "request_message.hpp"

namespace server
{
	Connection::Connection(int fd, const conf::VirtualServerConfs &conf, const SockAddrIn &client)
		: fd_(fd),
		  configs_(conf),
		  state_(kReceiving),
		  client_(client),
		  receiver_(fd, conf),
		  sender_(NULL)
	{}

	Connection::~Connection()
	{
		// close fdclass
		delete sender_;
	}

	PollInstructions Connection::Proceed()
	{
		std::list<io_multiplexer::PollInstruction> poll_instructions;

		switch (state_) {
		case kReceiving:
			poll_instructions = Receive();
		case kSending:
			poll_instructions = Send();
		}
		return poll_instructions;
	}

	PollInstructions Connection::Receive()
	{
		PollInstructions poll_instructions;

		receiver_.Proceed();
		if (receiver_.IsSuspending() || receiver_.IsFinished()) {
			const http::RequestMessage    &request = receiver_.GetRequest();
			const conf::VirtualServerConf &config  = configs_[request.GetHost()];
			sender_                                = new Sender(fd_, config, request, client_);
			poll_instructions -= receiver_;
			poll_instructions += sender_;
			state_ = kSending;
		}
		return poll_instructions;
	}

	PollInstructions Connection::Send()
	{
		// TODO httpmsg parse失敗時の分岐
		PollInstructions poll_instructions;
		poll_instructions += sender_->Proceed();
		if (sender_.IsFinished()) {
			poll_instructions -= sender_;
			if (receiver_.IsSuspending()) {
				poll_instructions += receiver_;
				state_ = kReceiving;
			} else if (receiver_.IsFinished()) {
				state_ = kFinished;
			}
			delete sender_;
			sender_ = NULL;
		}
		return poll_instructions;
	}

	bool Connection::IsFinished()
	{
		return state_ == kFinished;
	}
} // namespace server
