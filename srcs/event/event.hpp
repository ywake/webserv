#ifndef EVENT_HPP
#define EVENT_HPP

#include <stdint.h>

namespace event
{
	typedef struct Event {
		enum EventType {
			kWrite = 1U << 0,
			kRead  = 1U << 1,
		};

		int      fd;
		uint32_t event_type;
		void    *data;
	} Event;
} // namespace event

#endif
