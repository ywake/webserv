#ifndef FD_HPP
#define FD_HPP
#include <iostream>
#include <map>
#include <utility>

class Fd
{
  public:
	static const int kNofd = -2;

  protected:
	static std::map<int, std::size_t> fd_count_;
	int fd_;

  public:
	Fd();
	Fd(int fd);
	Fd(const Fd &copy);
	Fd &operator=(const Fd &other);
	bool operator<(const Fd &rhs) const;
	virtual ~Fd();

	// tmp
	int GetFd() const;
};
std::ostream &operator<<(std::ostream &out, const Fd &fd);

#endif
