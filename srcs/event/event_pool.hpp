#ifndef EVENT_POOL_HPP
#define EVENT_POOL_HPP

#include <deque>

#include "i_event.hpp"

namespace event
{
	class EventPool
	{
	  private:
		class Pool : public std::deque<IEvent *>
		{
		  public:
			void Push(IEvent *event)
			{
				push_back(event);
			}

			IEvent *Pop()
			{
				IEvent *ret = front();
				pop_front();
				return ret;
			}
		};

	  public:
		EventPool();
		void AddEvent(IEvent *event);
		void Run();
	};
} // namespace event
#endif
