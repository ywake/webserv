#include "fd.hpp"

#include <unistd.h>

std::map<int, std::size_t> Fd::fd_count_ = std::map<int, std::size_t>();

Fd::Fd() : fd_(kNofd) {}

Fd::Fd(int fd) : fd_(fd)
{
	if (fd < 0) {
		return;
	}
	fd_count_[fd_]++;
}

Fd::Fd(const Fd &copy) : fd_(copy.fd_)
{
	if (fd_ < 0) {
		return;
	}
	fd_count_[fd_]++;
}

Fd &Fd::operator=(const Fd &other)
{
	if (this == &other) {
		return *this;
	}
	fd_ = other.fd_;
	if (fd_ >= 0) {
		fd_count_[fd_]++;
	}
	return *this;
}

Fd::~Fd()
{
	if (fd_ < 0) {
		return;
	}
	fd_count_[fd_]--;
	if (fd_count_[fd_] == 0) {
		close(fd_);
	}
}

int Fd::GetFd() const
{
	return fd_;
}
