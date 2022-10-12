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
		void                            Proceed();
		const conf::VirtualServerConfs &configs_;
	};
} // namespace server
#endif
