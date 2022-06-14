#ifndef ISELECTOR_HPP
#define ISELECTOR_HPP

#include <map>
#include <queue>
#include <vector>

#include "event.hpp"
#include "result.hpp"

class ISelector
{
  protected:
	// typedef std::map<int, EventPool::Events >::iterator iterator;
	typedef std::map<int, std::queue<Event> >::iterator iterator;

  public:
	virtual Result<void> Import(iterator begin, iterator end) = 0;
	virtual Result<void> Run() = 0;
	virtual void Export(std::vector<int> &ready) = 0;
};

#endif
