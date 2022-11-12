#ifndef EPOLL_HPP
#define EPOLL_HPP

#include <list>
#include <map>
#include <sys/epoll.h>
#include <vector>

#include "event/event.hpp"
#include "event/instruction.hpp"
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
		int       epoll_fd_;
		int       timeout_;
		EventPool blocking_pool_;
		EventPool non_blocking_pool_;

	  public:
		Epoll(int timeout = -1);
		Result<event::Events> Wait();
		ErrEvents             InstructMulti(const event::Instructions &instructions);

	  private:
		Result<void>        Instruct(const event::Instruction &instruction);
		Result<void>        AppendEventType(const event::Event &event);
		Result<void>        TrimEventType(const event::Event &event);
		Result<void>        Register(const event::Event &event);
		Result<void>        Unregister(const event::Event &event);
		EpollEvent          ConvertToEpollEvent(const event::Event &event);
		event::Event        ConvertToEvent(const EpollEvent &epoll_event);
		event::Events       ConvertToEvents(const EpollEvents &epoll_events);
		Result<void>        InstructNonBlockingEvent(const event::Instruction &instruction);
		Result<void>        InstructBlockingEvent(const event::Instruction &instruction);
		Result<EpollEvents> WaitBlockingEvents();
		event::Events       ExportNonBlockingEvents();
		Result<void>        Overwrite(const event::Event &event);
	};
} // namespace io_multiplexer

#endif
