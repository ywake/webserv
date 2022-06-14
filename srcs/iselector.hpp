#ifndef ISELECTOR_HPP
#define ISELECTOR_HPP

#include <map>
#include <vector>

#include "event.hpp"
#include "result.hpp"

class ISelector
{
  protected:
	typedef std::map<int, Event>::iterator iterator;

  public:
	virtual Result<void> Import(iterator begin, iterator end) = 0;
	virtual Result<void> Run() = 0;
	virtual void Export(std::vector<int> &ready) = 0;
};

#endif
