#ifndef RECEIVER_HPP
#define RECEIVER_HPP

namespace server
{
	class Receiver
	{
	  private:
		int fd_;

	  public:
		void                            Proceed();
		const conf::VirtualServerConfs &configs_;
	};
} // namespace server
#endif
