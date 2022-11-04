#include "connection.hpp"
#include "request_message.hpp"
#include "webserv_utils.hpp"

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
			/*
			case kResourceReceiving:
				poll_instructions = resource_.Receive();
			case kResouceSending:
				poll_instructions = resource_.Send();
			*/
		}
		return poll_instructions;
	}

	PollInstructions Connection::Open()
	{
		PollInstructions poll_instructions;

		if (is_cgi) {
			resource_ = new Cgi(message_);
			resource_.SetMetaVariables("webserv", "8080", "192.168.0.22");
			resource_.Run("/usr/bin/python3");
		} else {
			resource_ = new RegularFile();
		}
		return poll_instructions;
	}

	PollInstructions Connection::Receive()
	{
		PollInstructions poll_instructions;

		poll_instrcutions += receiver_.Proceed();
		// TODO
		// クライアント側が接続を切っていて、recvがerrorで帰っていた場合、pollはそのコネクションに対してどんな挙動をるるかが不明
		// TODO
		// バッファにバイトが残っている場合、次の受信が来くるまでイベントが発火しない問題
		// エラーなバイト列を受信済みだったとしても、タイムアウトが返りそう
		if (receiver_.IsSuspending() || receiver_.IsFinished()) {
			const http::RequestMessage    &request = receiver_.GetRequest();
			const conf::VirtualServerConf &config  = configs_[request.GetHost()];
			sender_                                = new Sender(fd_, config, request, client_);
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
		if (sender_->IsFinished()) {
			if (receiver_.IsSuspending()) {
				poll_instructions += receiver_; // バッファあるとき無理かも
				state_ = kReceiving;
			} else if (receiver_.IsFinished()) {
				state_ = kFinished;
			}
			utils::DeleteSafe<Sender>(sender_);
		}
		return poll_instructions;
	}

	bool Connection::IsFinished()
	{
		return state_ == kFinished;
	}
} // namespace server
