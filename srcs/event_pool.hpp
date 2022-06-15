#ifndef EVENT_POOL_HPP
#define EVENT_POOL_HPP

#include <algorithm>
#include <cerrno>
#include <deque>
#include <map>
#include <sys/select.h>
#include <unistd.h>

#include "debug.hpp"
#include "event.hpp"
#include "iselector.hpp"
#include "result.hpp"
#include "state.hpp"

class EventPool
{
	typedef Event (*Callback)(int fd, Server *s);
	typedef std::deque<Event>::iterator iterator;

  private:
	std::deque<Event> pool_;
	std::map<State, State> state_chain_;
	std::map<State, Callback> callback_;
	fd_set ready;

  public:
	EventPool()
		: pool_(), state_chain_(), callback_(), ready()
	{
		state_chain_[A] = B;
		state_chain_[B] = C;
		state_chain_[C] = END;
		callback_[A] = OnAccept;
		callback_[B] = OnServe;
		callback_[C] = OnClose;
	};
	~EventPool(){};

	void Print()
	{
		for (iterator it = pool_.begin(); it != pool_.end(); it++) {
			Event event = *it;
			log("fd", event.fd_);
			log("state", event.state_);
		}
	}

	void Push(const Event &event)
	{
		if (event.state_ == END) {
			return;
		}
		pool_.push_back(event);
	}

	void Pop()
	{
		if (pool_.size() == 0) {
			return;
		}
		pool_.pop_front();
	}

	Result<void>
	MonitorFds(ISelector *selector)
	{
		selector->Import(pool_.begin(), pool_.end()); //[TODO] allocate失敗
		Result<void> res = selector->Run();
		if (res.IsErr()) {
			return res;
		}
		selector->Export(&ready);
		return Result<void>();
	}

	Event Run(Event &event)
	{
		Callback func = callback_[event.state_];
		return func(event.fd_, event.server_);
	}

	void FlowPool()
	{
		size_t size = pool_.size();
		for (size_t i = 0; i < size; i++) {
			Event event = pool_.front();
			Pop();
			if (FD_ISSET(event.fd_, &ready)) {
				log("fd", event.fd_);
				log("state", event.state_);
				Event res = Run(event);
				State next = state_chain_[res.state_];
				if (res.state_ == A) {
					Push(event);
				}
				event = Event(res.fd_, res.server_, next);
			}
			Push(event);
		}
	}
};

#endif
