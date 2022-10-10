#ifndef I_EVENT_HPP
#define I_EVENT_HPP

namespace event
{
	class IEvent
	{
		virtual void Run() = 0;
	};
} // namespace event

#endif
