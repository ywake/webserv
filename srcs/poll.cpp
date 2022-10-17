#include "poll.hpp"

#include <cerrno>
#include <cstring>
#include <sys/epoll.h>

namespace io_multiplexer
{
	Poll::Poll(int timeout) : timeout_(timeout), registered_events_()
	{
		epoll_fd_ = epoll_create1(EPOLL_CLOEXEC);
		if (epoll_fd_ == -1) {
			throw std::runtime_error(std::string("epoll_create: ") + strerror(errno));
		}
	}

	Result<PollEvents> Poll::Wait()
	{
		int                 num_of_events = registered_events_.size();
		PollEvents          events(num_of_events);
		struct epoll_event *events_ptr  = events.data();
		int                 events_size = 0;

		while (true) {
			events_size = epoll_wait(epoll_fd_, events_ptr, num_of_events, timeout_);
			if (events_size != -1) {
				break;
			} else if (errno == EINTR)
				continue;
			else {
				return Error(std::string("epoll_wait: ") + strerror(errno));
			}
		}
		events.resize(events_size);
		return events;
	}

	ErrEvents Poll::InstructMulti(const PollInstructions &instructions)
	{
		ErrEvents err_events;

		for (PollInstructions::const_iterator it = instructions.begin(); it != instructions.end();
			 it++) {
			Result<void> res = Instruct(*it);
			if (res.IsErr()) {
				err_events.push_back((*it, Error(res.Err())));
			}
		}
		return err_events;
	}

	Result<void> Poll::Instruct(const PollInstruction &instruction)
	{
		switch (instruction.command) {
		case PollInstruction::kAppendEventType:
			return AppendEventType(instruction);
		case PollInstruction::kTrimEventType:
			return TrimEventType(instruction);
		case PollInstruction::kRegister:
			return Register(instruction.fd, instruction.event);
		case PollInstruction::kUnregister:
			return Unregister(instruction.fd);
		}
		return Result<void>();
	}

	Result<void> Poll::AppendEventType(const PollInstruction &instruction)
	{
		Result<void>       res;
		struct epoll_event event = {};

		try {
			event = registered_events_.at(instruction.fd);
		} catch (const std::out_of_range &e) {
			(void)e;
		}
		// TODO operator=
		res = Unregister(instruction.fd);
		if (res.IsErr()) {
			return res;
		}
		event.events |= instruction.event.events;
		event.data = instruction.event.data; //これどうするか微妙
		res        = Register(instruction.fd, event);
		if (res.IsErr()) {
			return res;
		}
		return Result<void>();
	}

	Result<void> Poll::TrimEventType(const PollInstruction &instruction)
	{
		Result<void>       res;
		struct epoll_event event = {};

		try {
			event = registered_events_.at(instruction.fd);
		} catch (const std::out_of_range &e) {
			(void)e;
		}
		res = Unregister(instruction.fd);
		if (res.IsErr()) {
			return res;
		}
		event.events &= ~instruction.event.events;
		if (!event.events) {
			return Result<void>();
		}
		event.data = instruction.event.data; //これどうするか微妙
		res        = Register(instruction.fd, event);
		if (res.IsErr()) {
			return res;
		}
		return Result<void>();
	}

	Result<void> Poll::Register(int fd, const struct epoll_event &event)
	{
		struct epoll_event ev = event;
		if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev) == -1) {
			return Error(std::string("epoll append2: ") + strerror(errno));
		}
		registered_events_[fd] = event;
		return Result<void>();
	}

	Result<void> Poll::Unregister(int fd)
	{
		struct epoll_event unused;

		if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, &unused) == -1) {
			if (errno == ENOENT) {
				return Result<void>();
			}
			return Error(std::string("epoll unregister: ") + strerror(errno));
		}
		registered_events_.erase(fd);
		return Result<void>();
	}

} // namespace io_multiplexer
