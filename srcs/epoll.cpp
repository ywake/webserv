#ifdef __linux__

#include "epoll.hpp"

#include <cerrno>
#include <cstring>

#include "webserv_utils.hpp"

// TODO 何もテストしてない
namespace io_multiplexer
{
	Epoll::Epoll(int timeout) : timeout_(timeout), blocking_pool_()
	{
		int epfd = epoll_create1(EPOLL_CLOEXEC);
		if (epfd == -1) {
			throw std::runtime_error(std::string("epoll_create: ") + strerror(errno));
		}
		epoll_fd_ = epfd;
	}

	Result<event::Events> Epoll::Wait()
	{
		event::Events       events          = ExportNonBlockingEvents();
		int                 timeout         = events.empty() ? timeout_ : 0;
		Result<EpollEvents> wait_result     = WaitBlockingEvents(timeout);
		event::Events       blocking_events = ConvertToEvents(wait_result.Val());
		events.splice(events.end(), blocking_events);
		return Result<event::Events>(events, Error(wait_result.Err()));
	}

	Result<Epoll::EpollEvents> Epoll::WaitBlockingEvents(int timeout)
	{
		int         num_of_events = blocking_pool_.size();
		EpollEvents events(num_of_events);
		EpollEvent *events_ptr = events.data();

		while (true) {
			int events_size = epoll_wait(epoll_fd_.GetFd(), events_ptr, num_of_events, timeout);
			if (events_size != -1) {
				events.resize(events_size);
				return events;
			} else if (errno == EINTR)
				continue;
			else {
				return Error(std::string("epoll_wait: ") + strerror(errno));
			}
		}
	}

	event::Events Epoll::ExportNonBlockingEvents()
	{
		event::Events events;

		for (EventPool::const_iterator it = non_blocking_pool_.begin();
			 it != non_blocking_pool_.end();
			 ++it) {
			const event::Event &event = it->second;
			if (event.event_type != event::Event::kEmpty) {
				events.push_back(event);
			}
		}
		return events;
	}

	ErrEvents Epoll::InstructMulti(const event::Instructions &instructions)
	{
		ErrEvents err_events;

		for (event::Instructions::const_iterator it = instructions.begin();
			 it != instructions.end();
			 it++) {
			Result<void> res = Instruct(*it);
			if (res.IsErr()) {
				err_events.push_back((*it, Error(res.Err())));
			}
		}
		return err_events;
	}

	Result<void> Epoll::Instruct(const event::Instruction &instruction)
	{
		switch (instruction.command) {
		case event::Instruction::kAppendEventType:
			return AppendEventType(instruction.event);
		case event::Instruction::kTrimEventType:
			return TrimEventType(instruction.event);
		case event::Instruction::kRegister:
			return Register(instruction.event);
		case event::Instruction::kUnregister:
			return Unregister(instruction.event);
		}
		return Result<void>();
	}

	Result<void> Epoll::AppendEventType(const event::Event &event)
	{
		EventPool::iterator it;

		it = non_blocking_pool_.find(event.fd);
		if (it != non_blocking_pool_.end()) {
			event::Event &base_event = it->second;
			base_event.event_type |= event.event_type;
			return Result<void>();
		}
		it = blocking_pool_.find(event.fd);
		if (it != blocking_pool_.end()) {
			event::Event new_event = it->second;
			new_event.event_type |= event.event_type;
			return Overwrite(new_event);
		}
		return Error("no entry in event pool");
	}

	Result<void> Epoll::TrimEventType(const event::Event &event)
	{
		EventPool::iterator it;

		it = non_blocking_pool_.find(event.fd);
		if (it != non_blocking_pool_.end()) {
			event::Event &base_event = it->second;
			base_event.event_type &= ~event.event_type;
			return Result<void>();
		}
		it = blocking_pool_.find(event.fd);
		if (it != blocking_pool_.end()) {
			event::Event new_event = it->second;
			new_event.event_type &= ~event.event_type;
			return Overwrite(new_event);
		}
		return Error("no entry in event pool");
	}

	Result<void> Epoll::Register(const event::Event &event)
	{
		Result<bool> res = utils::IsRegularFile(event.fd);
		if (res.IsErr()) {
			return res.Err();
		}
		const bool is_regular = res.Val();
		if (is_regular) {
			non_blocking_pool_[event.fd] = event;
			return Result<void>();
		} else {
			return RegisterBlockingEvent(event);
		}
	}

	Result<void> Epoll::RegisterBlockingEvent(const event::Event &event)
	{
		EpollEvent ev = ConvertToEpollEvent(event);

		if (epoll_ctl(epoll_fd_.GetFd(), EPOLL_CTL_ADD, event.fd, &ev) == -1) {
			return Error(std::string("epoll add: ") + strerror(errno));
		} // TODO ENOMEM
		blocking_pool_[event.fd] = event;
		return Result<void>();
	}

	Result<void> Epoll::Unregister(const event::Event &event)
	{
		EventPool::iterator it;

		it = non_blocking_pool_.find(event.fd);
		if (it != non_blocking_pool_.end()) {
			non_blocking_pool_.erase(event.fd);
			return Result<void>();
		}
		it = blocking_pool_.find(event.fd);
		if (it != blocking_pool_.end()) {
			return UnregisterBlockingEvent(event);
		}
		return Error("no entry in event pool");
	}

	Result<void> Epoll::UnregisterBlockingEvent(const event::Event &event)
	{
		blocking_pool_.erase(event.fd);
		EpollEvent unused;
		if (epoll_ctl(epoll_fd_.GetFd(), EPOLL_CTL_DEL, event.fd, &unused) == -1) {
			return Error(std::string("epoll delete: ") + strerror(errno));
		} // TODO ENOMEM
		return Result<void>();
	}

	Result<void> Epoll::Overwrite(const event::Event &event)
	{
		EpollEvent ev = ConvertToEpollEvent(event);

		if (epoll_ctl(epoll_fd_.GetFd(), EPOLL_CTL_MOD, event.fd, &ev) == -1) {
			return Error(std::string("epoll modify: ") + strerror(errno));
		} // TODO ENOMEM
		blocking_pool_[event.fd] = event;
		return Result<void>();
	}

	EpollEvent Epoll::ConvertToEpollEvent(const event::Event &event)
	{
		EpollEvent epoll_event = {};

		epoll_event.data.ptr = event.data;
		if (event.event_type & event::Event::kWrite) {
			epoll_event.events |= EPOLLOUT;
		}
		if (event.event_type & event::Event::kRead) {
			epoll_event.events |= EPOLLIN;
		}
		return epoll_event;
	}

	event::Events Epoll::ConvertToEvents(const EpollEvents &epoll_events)
	{
		event::Events events;

		for (EpollEvents::const_iterator it = epoll_events.begin(); it != epoll_events.end();
			 ++it) {
			const EpollEvent &epoll_event = *it;
			int               fd          = epoll_event.data.fd;
			event::Event      ev          = blocking_pool_[fd];
			if (epoll_event.events & EPOLLOUT) {
				ev.event_type |= event::Event::kWrite;
			}
			if (epoll_event.events & EPOLLIN) {
				ev.event_type |= event::Event::kRead;
			}
			events.push_back(ev);
		}
		return events;
	}

} // namespace io_multiplexer

#endif
