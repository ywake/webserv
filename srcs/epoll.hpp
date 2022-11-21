#ifndef EPOLL_HPP
#define EPOLL_HPP

#ifdef __linux__

#include <list>
#include <map>
#include <sys/epoll.h>
#include <vector>

#include "event/event.hpp"
#include "event/instruction.hpp"
#include "managed_fd.hpp"
#include "result.hpp"

namespace io_multiplexer
{
	typedef struct ::epoll_event                         EpollEvent;
	typedef std::vector<EpollEvent>                      EpollEvents;
	typedef std::list<Result<const event::Instruction> > ErrEvents;

	class Epoll
	{
	  private:
		typedef std::map<int, event::Event> EventPool;

	  private:
		ManagedFd epoll_fd_;
		int       timeout_;
		EventPool blocking_pool_;
		EventPool non_blocking_pool_;

	  public:
		Epoll(int timeout = -1);
		Result<event::Events> Wait();
		ErrEvents             InstructMulti(const event::Instructions &instructions);
		Result<void>          Instruct(const event::Instruction &instruction);
		Result<void>          AppendEventType(const event::Event &event);
		Result<void>          TrimEventType(const event::Event &event);
		Result<void>          Register(const event::Event &event);
		Result<void>          Unregister(const event::Event &event);

	  private:
		Result<void>        RegisterBlockingEvent(const event::Event &event);
		Result<void>        UnregisterBlockingEvent(const event::Event &event);
		Result<void>        Overwrite(const event::Event &event);
		Result<EpollEvents> WaitBlockingEvents(int timeout);
		event::Events       ExportNonBlockingEvents();
		EpollEvent          ConvertToEpollEvent(const event::Event &event);
		event::Event        ConvertToEvent(const EpollEvent &epoll_event);
		event::Events       ConvertToEvents(const EpollEvents &epoll_events);
	};
} // namespace io_multiplexer

#endif

#endif
