#include "managed_fd.hpp"

#include <cassert>

#include "webserv_utils.hpp"

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
	CountDown(fd_);
	fd_ = other.fd_;
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

void ManagedFd::Close()
{
	CountDown(fd_);
	fd_ = kNofd;
}

void ManagedFd::CountUp(int fd)
{
	if (fd < 0) {
		return;
	}
	fd_count_[fd]++;
}

void ManagedFd::CountDown(int fd)
{
	if (fd < 0) {
		return;
	}
	FdCounter::iterator it = fd_count_.find(fd);
	assert(it != fd_count_.end());

	std::size_t counter = --it->second;
	if (counter > 0) {
		return;
	}
	fd_count_.erase(fd);
	utils::Close(fd);
}

void ManagedFd::CloseOther()
{
	for (FdCounter::iterator it = fd_count_.begin(); it != fd_count_.end(); ++it) {
		int fd = it->first;
		if (fd != fd_) {
			utils::Close(fd);
		}
	}
}
