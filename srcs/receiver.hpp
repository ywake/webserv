#ifndef RECEIVER_HPP
#define RECEIVER_HPP

#include "request_message.hpp"

namespace server
{
	class Receiver
	{
	  private:
		enum State {
			kRequsetLine,
			kHeaderSection,
			kBody,
			kClose,
			kKeepAlive,
		};

	  private:
		int                  fd_;
		State                state_;
		http::RequestMessage request_;

	  private:
		Receiver();

	  public:
		Receiver(int fd, const conf::VirtualServerConfs &configs);
		PollInstructions                Proceed();
		const conf::VirtualServerConfs &configs_;
		const http::RequestMessage     &GetRequest() const;
		bool                            IsSuspending();
		bool                            IsFinished();
	};
} // namespace server
#endif
