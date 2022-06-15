#ifndef ISELECTOR_HPP
#define ISELECTOR_HPP

#include <map>
#include <vector>
#include <deque>

#include "event.hpp"
#include "result.hpp"

class ISelector
{
  protected:
	typedef std::deque<Event>::iterator iterator;

  public:
	virtual Result<void> Import(iterator begin, iterator end) = 0;
	virtual Result<void> Run() = 0;
	virtual void Export(fd_set* ready) = 0;
};

#endif
