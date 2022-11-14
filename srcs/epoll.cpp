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
		epoll_fd_ = epoll_create1(EPOLL_CLOEXEC);
		if (epoll_fd_ == -1) {
			throw std::runtime_error(std::string("epoll_create: ") + strerror(errno));
		}
	}

	Result<event::Events> Epoll::Wait()
	{
		Result<EpollEvents> wait_result     = WaitBlockingEvents();
		event::Events       blocking_events = ConvertToEvents(wait_result.Val());
		event::Events       events          = ExportNonBlockingEvents();
		events.splice(events.end(), blocking_events);
		return Result<event::Events>(events, Error(wait_result.Err()));
	}

	Result<EpollEvents> Epoll::WaitBlockingEvents()
	{
		int         num_of_events = blocking_pool_.size();
		int         timeout       = non_blocking_pool_.empty() ? timeout_ : 0;
		EpollEvents events(num_of_events);
		EpollEvent *events_ptr = events.data();

		while (true) {
			int events_size = epoll_wait(epoll_fd_, events_ptr, num_of_events, timeout);
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
		Result<bool> res = utils::IsRegularFile(instruction.event.fd);
		if (res.IsErr()) {
			return Error(res.Err());
		}
		const bool is_regular = res.Val();
		if (is_regular) {
			return InstructNonBlockingEvent(instruction);
		} else {
			return InstructBlockingEvent(instruction);
		}
	}

	Result<void> Epoll::InstructNonBlockingEvent(const event::Instruction &instruction)
	{
		try {
			const event::Event &event = instruction.event;

			switch (instruction.command) {
			case event::Instruction::kAppendEventType:
				non_blocking_pool_.at(event.fd).event_type |= event.event_type;
				break;
			case event::Instruction::kTrimEventType:
				non_blocking_pool_.at(event.fd).event_type &= ~event.event_type;
				break;
			case event::Instruction::kRegister:
				blocking_pool_[event.fd] = event;
				break;
			case event::Instruction::kUnregister:
				non_blocking_pool_.erase(event.fd);
				break;
			}
			return Result<void>();
		} catch (const std::out_of_range &e) {
			return Error("no entry in non-blocking event");
		}
	}

	Result<void> Epoll::InstructBlockingEvent(const event::Instruction &instruction)
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
		try {
			event::Event new_event = blocking_pool_.at(event.fd);
			new_event.event_type |= event.event_type;
			return Overwrite(new_event);
		} catch (const std::out_of_range &e) {
			return Error("no entry in blocking event");
		}
	}

	Result<void> Epoll::TrimEventType(const event::Event &event)
	{
		try {
			event::Event new_event = blocking_pool_.at(event.fd);
			new_event.event_type &= ~event.event_type;
			return Overwrite(new_event);
		} catch (const std::out_of_range &e) {
			return Error("no entry in blocking event");
		}
	}

	Result<void> Epoll::Register(const event::Event &event)
	{
		EpollEvent ev = ConvertToEpollEvent(event);

		if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, event.fd, &ev) == -1) {
			return Error(std::string("epoll add: ") + strerror(errno));
		}
		blocking_pool_[event.fd] = event;
		return Result<void>();
	}

	Result<void> Epoll::Overwrite(const event::Event &event)
	{
		EpollEvent ev = ConvertToEpollEvent(event);

		if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, event.fd, &ev) == -1) {
			return Error(std::string("epoll modify: ") + strerror(errno));
		}
		blocking_pool_[event.fd] = event;
		return Result<void>();
	}

	Result<void> Epoll::Unregister(const event::Event &event)
	{
		EpollEvent unused;

		if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, event.fd, &unused) == -1) {
			return Error(std::string("epoll delete: ") + strerror(errno));
		}
		blocking_pool_.erase(event.fd);
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

	event::Event Epoll::ConvertToEvent(const EpollEvent &epoll_event)
	{
		event::Event event = {};

		event.data = epoll_event.data.ptr;
		if (epoll_event.events & EPOLLOUT) {
			event.event_type |= event::Event::kWrite;
		}
		if (epoll_event.events & EPOLLIN) {
			event.event_type |= event::Event::kRead;
		}
		return event;
	}

	event::Events Epoll::ConvertToEvents(const EpollEvents &epoll_events)
	{
		event::Events events;

		for (EpollEvents::const_iterator it = epoll_events.begin(); it != epoll_events.end();
			 ++it) {
			const EpollEvent &epoll_event = *it;
			events.push_back(ConvertToEvent(epoll_event));
		}
		return events;
	}

} // namespace io_multiplexer

#endif
