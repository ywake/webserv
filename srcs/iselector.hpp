#ifndef ISELECTOR_HPP
#define ISELECTOR_HPP

#include <map>

#include "event.hpp"
#include "result.hpp"

class ISelector
{
  protected:
	typedef std::map<int, Event>::iterator iterator;

  public:
	virtual Result<void> Import(iterator begin, iterator end) = 0;
	virtual Result<void> Run() = 0;
	virtual const std::vector<int> &Export() = 0;
};

#endif
