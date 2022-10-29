#ifndef EVENT_HPP
#define EVENT_HPP

namespace event
{
	typedef struct Event {
		enum EventType {
			kWrite,
			kRead,
		};
		int       fd;
		void     *event;
		EventType event_type;
	} Event;
} // namespace event

#endif
