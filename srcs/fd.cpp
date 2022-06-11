#include <unistd.h>

#include "fd.hpp"

std::map< int, std::size_t > Fd::fd_count_ = std::map< int, std::size_t >();

Fd::Fd()
	: fd_(kNofd) {}

Fd::Fd(int fd)
	: fd_(fd)
{
	fd_count_[fd_]++;
}

Fd::Fd(const Fd &copy)
	: fd_(copy.fd_)
{
	fd_count_[fd_]++;
}

Fd &Fd::operator=(const Fd &other)
{
	if (this == &other) {
		return *this;
	}
	fd_ = other.fd_;
	fd_count_[fd_]++;
	return *this;
}

Result< void > Fd::CreateOnce()
{
	if (fd_ != kNofd) {
		return Result< void >();
	}
	Result< void > res = Create();
	if (res.IsOk()) {
		fd_count_[fd_]++;
	}
	return res;
}

Fd::~Fd()
{
	if (--fd_count_[fd_] == 0 && fd_ != kNofd) {
		close(fd_);
	}
}

int Fd::getFd()
{
	return fd_;
}
