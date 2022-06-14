#ifndef ISELECTOR_HPP
#define ISELECTOR_HPP

#include <map>

#include "fd.hpp"
#include "result.hpp"
#include "state.hpp"

class ISelector
{
  protected:
	typedef std::map<Fd, State::FdState>::iterator iterator;

  public:
	virtual Result<void> Import(iterator begin, iterator end) = 0;
	virtual Result<void> Run() = 0;
	virtual const std::vector<int> &Export() = 0;
};

#endif
