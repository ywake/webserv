#ifndef FDSET_HPP
#define FDSET_HPP

#include <stdint.h>
#include <vector>

class FdSet
{
  private:
	std::vector<uint64_t> ready_fds_;

  public:
	FdSet();
	FdSet(const FdSet &copy);
	const FdSet &operator=(const FdSet &other);
	~FdSet();

	void SetFd(int fd);
	bool IsReady(int fd);
};

#endif
