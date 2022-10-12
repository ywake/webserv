#include "connection.hpp"
#include "request_message.hpp"

namespace server
{
	Connection::Connection(int fd, const conf::VirtualServerConfs &conf, const SockAddrIn &client)
		: fd_(fd), configs_(conf), state_(kReceiving), client_(client)
	{}

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
			const RequestMessage          &msg    = receiver_.GetRequestMessage();
			const conf::VirtualServerConf &config = configs_[msg.GetHost()];
			sender_                               = Sender(fd_, config, client_, msg);
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
		poll_instructions += sender_.Proceed();
		if (sender_.IsFinished()) {
			poll_instructions -= sender_;
			if (receiver_.IsSuspending()) {
				poll_instructions += receiver_;
				state_ = kReceiving;
			} else if (receiver_.IsFinished()) {
				state_ = kFinished;
			}
		}
		return poll_instructions;
	}

	bool Connection::IsFinished()
	{
		return state_ == kFinished;
	}
} // namespace server
