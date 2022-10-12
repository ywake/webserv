#ifndef RECEIVER_HPP
#define RECEIVER_HPP

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
		int   fd_;
		State state_;

	  public:
		void                            Proceed();
		const conf::VirtualServerConfs &configs_;
	};
} // namespace server
#endif
