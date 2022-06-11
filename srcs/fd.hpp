#ifndef FD_HPP
#define FD_HPP
#include <map>
#include <utility>

class Fd
{
  public:
	static const int kNofd = -2;

  protected:
	static std::map< int, std::size_t > fd_count_;
	int fd_;

  public:
	Fd();
	Fd(int fd);
	Fd(const Fd &sock);
	Fd &operator=(const Fd &sock);
	virtual ~Fd();

	// tmp
	int GetFd() const;
};

#endif
