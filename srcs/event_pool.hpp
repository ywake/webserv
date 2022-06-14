#ifndef EVENT_POOL_HPP
#define EVENT_POOL_HPP

#include <algorithm>
#include <cerrno>
#include <map>
#include <sys/select.h>
#include <unistd.h>

#include "callback.hpp"
#include "debug.hpp"
#include "fd.hpp"
#include "iselector.hpp"
#include "result.hpp"
#include "state.hpp"
class EventPool
{
  private:
	std::map<Fd, State::FdState> state_map_;
	std::map<State::State, Callback::Callback> event_map_;

  public:
	EventPool()
		: state_map_()
	{
		event_map_[State::LISTEN] = Callback::Accept;
		event_map_[State::RECV] = Callback::Serve;
		event_map_[State::SEND] = Callback::Serve;
	};
	~EventPool(){};
	void UpdateState(const State::FdState &state)
	{
		switch (state.state_) {
		case State::END:
			state_map_.erase(state.fd_);
			log(state.fd_, "event stop");
			break;
		default:
			state_map_[state.fd_] = state;
			log(state.fd_, "event add");
			break;
		}
	}

	Result<std::vector<int> > MonitorFds(ISelector *selector)
	{
		selector->Import(state_map_.begin(), state_map_.end()); //[TODO] allocate失敗
		Result<void> res = selector->Run();
		if (res.IsErr()) {
			return Result<std::vector<int> >(res.err);
		}
		return selector->Export();
	}

	void TriggerEvents(const std::vector<int> &ready)
	{
		typedef std::vector<int>::const_iterator iterator;

		for (iterator it = ready.begin(); it != ready.end(); it++) {
			int fd = *it;
			State::FdState state = state_map_[fd];
			Callback::Callback event = event_map_[state.state_];
			State::FdState new_state = event(fd, state.server_);
			UpdateState(new_state);
		}
	}
};

#endif
