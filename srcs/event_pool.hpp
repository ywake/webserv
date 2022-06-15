#ifndef EVENT_POOL_HPP
#define EVENT_POOL_HPP

#include <cerrno>
#include <deque>
#include <map>

#include "debug.hpp"
#include "event.hpp"
#include "event_result.hpp"
#include "iselector.hpp"
#include "result.hpp"
#include "state.hpp"

class EventPool
{
  private:
	class Pool : public std::deque<Event>
	{
	  public:
		void Push(const Event &event)
		{
			push_back(event);
		}

		Event Pop()
		{
			Event ret = front();
			pop_front();
			return ret;
		}
	};
	typedef Pool::iterator iterator;
	Pool wait_pool_;
	Pool ready_pool_;
	std::map<State, State> state_chain_;
	std::map<State, Callback> callback_;
	fd_set ready;

  public:
	EventPool()
		: wait_pool_(), ready_pool_(), state_chain_(), callback_(), ready()
	{
		state_chain_[ACCEPT] = SERVE;
		state_chain_[SERVE] = CLOSE;
		state_chain_[CLOSE] = END;

		callback_[ACCEPT] = OnAccept;
		callback_[SERVE] = OnServe;
		callback_[CLOSE] = OnClose;
	};
	~EventPool(){};

	Result<void> MonitorFds(ISelector *selector)
	{
		selector->Import(wait_pool_.begin(), wait_pool_.end()); //[TODO] allocate失敗
		Result<void> res = selector->Run();
		if (res.IsErr()) {
			return res;
		}
		selector->Export(&ready);
		return Result<void>();
	}

	void TransitionEvents()
	{
		size_t size = wait_pool_.size();
		for (size_t i = 0; i < size; i++) {
			Event event = wait_pool_.Pop();
			log("fd", event.fd_);
			if (FD_ISSET(event.fd_, &ready)) {
				ready_pool_.Push(event);
			} else {
				wait_pool_.Push(event);
			}
		}
	}

	void AddEvent(const EventResult &res)
	{
		State next = state_chain_[res.state_];
		if (next == END) {
			return;
		}
		Callback func = callback_[next];
		wait_pool_.Push(Event(res.fd_, res.server_, func));
	}

	void AddEvent(const Event &event)
	{
		wait_pool_.Push(event);
	}

	void RunReadyEvents()
	{
		while (!ready_pool_.empty()) {
			Event event = ready_pool_.Pop();
			EventResult res = event.Run();
			if (res.state_ == ACCEPT) {
				AddEvent(event);
			}
			AddEvent(res);
		}
	}
};

#endif
