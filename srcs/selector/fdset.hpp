#ifndef FDSET_HPP
#define FDSET_HPP

#include <limits.h>
#include <stdint.h>
#include <unistd.h>
#include <vector>

class FdSet
{
  private:
	std::vector<uint64_t> ready_fds_;
	static const size_t kBits = sizeof(uint64_t) * CHAR_BIT;

  public:
	FdSet();
	FdSet(const FdSet &copy);
	const FdSet &operator=(const FdSet &other);
	~FdSet();

	void SetFd(int fd);
	bool IsReady(int fd);
	void Reset();
};

#endif
