#include "managed_fd.hpp"

#include <cassert>
#include <cerrno>
#include <stdio.h>
#include <unistd.h>

#include "file_close_exception.hpp"

ManagedFd::FdCounter ManagedFd::fd_count_ = std::map<int, std::size_t>();

ManagedFd::ManagedFd() : fd_(kNofd) {}

ManagedFd::ManagedFd(int fd) : fd_(fd)
{
	CountUp(fd_);
}

ManagedFd::ManagedFd(const ManagedFd &copy) : fd_(copy.fd_)
{
	CountUp(fd_);
}

ManagedFd &ManagedFd::operator=(const ManagedFd &other)
{
	if (this == &other) {
		return *this;
	}
	CountUp(other.fd_);
	Result<void> res = CountDown(fd_);
	fd_              = other.fd_;
	if (res.IsErr()) {
		throw FileCloseException(res.ErrMsg());
	}
	return *this;
}

ManagedFd::~ManagedFd()
{
	CountDown(fd_);
}

int ManagedFd::GetFd() const
{
	return fd_;
}

void ManagedFd::CountUp(int fd)
{
	if (fd < 0) {
		return;
	}
	fd_count_[fd]++;
}

Result<void> ManagedFd::CountDown(int fd)
{
	if (fd < 0) {
		return Result<void>();
	}
	FdCounter::iterator it = fd_count_.find(fd);
	assert(it != fd_count_.end());

	std::size_t counter = --it->second;
	if (counter > 0) {
		return Result<void>();
	}
	fd_count_.erase(fd);
	if (close(fd) < 0) {
		return Error(std::string("close: ") + strerror(errno));
	}
	return Result<void>();
}
