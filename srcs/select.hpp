#ifndef SELECT_HPP
#define SELECT_HPP

#include <sys/select.h>
#include <vector>

#include "iselector.hpp"

class Select : public ISelector
{
  private:
	int nfds_;
	fd_set read_set_;
	fd_set ready_set_;

  public:
	Select();
	~Select();
	Select(Select const &copy);
	Select &operator=(Select const &other);
	Result<void> Import(iterator begin, iterator end);
	Result<void> Run();
	void Export(std::vector<int> &ready);
};

#endif
