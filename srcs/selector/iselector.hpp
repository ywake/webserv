#ifndef ISELECTOR_HPP
#define ISELECTOR_HPP

#include <deque>
#include <map>
#include <queue>
#include <vector>

#include "event.hpp"
#include "fdset.hpp"
#include "result.hpp"

class ISelector
{
  protected:
	typedef std::deque<Event>::iterator iterator;

  public:
	virtual Result<void> Import(iterator begin, iterator end) = 0;
	virtual Result<void> Run()								  = 0;
	virtual void		 Export(FdSet &ready)				  = 0;
};

#endif
