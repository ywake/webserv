#ifndef FD_HPP
#define FD_HPP

#include <climits>
#include <map>

#include "result.hpp"

class ManagedFd
{
  private:
	typedef std::map<int, std::size_t> FdCounter;

  public:
	static const int kNofd = INT_MIN;

  protected:
	static FdCounter fd_count_;
	int              fd_;

  public:
	ManagedFd();
	ManagedFd(int fd);
	ManagedFd(const ManagedFd &sock);
	ManagedFd &operator=(const ManagedFd &sock);
	virtual ~ManagedFd();

	// tmp
	int GetFd() const;

  private:
	void CountUp(int fd);
	void CountDown(int fd);
};

#endif
