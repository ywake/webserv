#ifndef FD_EVENT_HPP
#define FD_EVENT_HPP

namespace io_multiplexer
{
	typedef struct FdEvent {
		enum EventType {
			kWrite,
			kRead,
		};
		int       fd;
		void     *event;
		EventType event_type;
	} FdEvent;
} // namespace io_multiplexer

#endif
