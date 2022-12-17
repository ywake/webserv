#ifndef EVENT_HPP
#define EVENT_HPP

#include <iostream>
#include <list>
#include <stdint.h>
namespace event
{
	typedef struct Event     Event;
	typedef std::list<Event> Events;
	typedef struct Event {
		enum EventType {
			kEmpty  = 0,
			kWrite  = 1U << 0,
			kRead   = 1U << 1,
			kHangUp = 1U << 2,
			kError  = 1U << 3
		};

		int      fd;
		void    *data;
		uint32_t event_type;
	} Event;
} // namespace event

std::ostream &operator<<(std::ostream &os, const event::Event &event);

#endif
