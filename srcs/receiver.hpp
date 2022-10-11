#ifndef RECEIVER_HPP
#define RECEIVER_HPP

namespace server
{
	class Receiver
	{
	  private:
		int fd_;

	  public:
		void Proceed();
	};
} // namespace server
#endif
