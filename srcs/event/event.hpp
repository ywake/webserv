#ifndef EVENT_HPP
#define EVENT_HPP

#include <list>
#include <stdint.h>
namespace event
{
	typedef struct Event {
		enum EventType {
			kWrite = 1U << 0,
			kRead  = 1U << 1
		};
		int                   fd;
		void                 *data;
		uint32_t              event_type;
		static const uint32_t kEmpty = 0;
	} Event;

	typedef std::list<Event> Events;
} // namespace event

#endif
