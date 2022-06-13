#ifndef EVENT_POOL_HPP
#define EVENT_POOL_HPP

#include <algorithm>
#include <map>
#include <sys/select.h>
#include <unistd.h>

#include "debug.hpp"
#include "event.hpp"

typedef struct {
	int nfds;
	fd_set read_set;
	// int nb_read_set;
	// fd_set write_set;
} SelectFds;

class EventPool
{
	typedef std::map<int, Event>::iterator iterator;

  private:
	std::map<int, Event> pool_;

  public:
	EventPool()
		: pool_(){};
	~EventPool(){};
	void AddEvent(Event e)
	{
		log(e.fd_, "event add");
		pool_[e.fd_] = e;
	}

	void CollectFds(SelectFds *set)
	{
		FD_ZERO(&set->read_set);
		set->nfds = 0;
		for (iterator it = pool_.begin(); it != pool_.end(); ++it) {
			int fd = (*it).first;

			FD_SET(fd, &set->read_set);
			set->nfds = std::max(set->nfds, fd + 1);
		}
	}

	void EventsTrigger(SelectFds &set)
	{
		for (iterator it = pool_.begin(); it != pool_.end();) {
			int fd = (*it).first;
			Event event = (*it).second;

			if (FD_ISSET(fd, &set.read_set)) {
				std::vector<Event> res = event.Run();
				iterator tmp = it;
				++tmp;
				log(fd, "drop event");
				pool_.erase(fd);
				it = tmp;
				for (std::vector<Event>::iterator it = res.begin(); it != res.end(); ++it) {
					Event e = *it;
					AddEvent(e);
				}
			} else {
				++it;
			}

			// if (FD_ISSET(fd, &set->write_set)) {
			// 	event.Run();
			// }
		}
	}
};

#endif
