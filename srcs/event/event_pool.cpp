#include "event_pool.hpp"

namespace event
{
	EventPool::EventPool() : pool_() {}

	void EventPool::AddEvent(IEvent *event)
	{
		pool_.PushBack(event);
	}

	void EventPool::Run()
	{
		while (!pool_.empty()) {
			IEvent *event = pool_.PopFront();
			event->Run();
		}
	}

	void EventPool::Pool::PushBack(IEvent *event)
	{
		push_back(event);
	}

	IEvent *EventPool::Pool::PopFront()
	{
		IEvent *ret = front();
		pop_front();
		return ret;
	}

} // namespace event
