#include "managed_fd.hpp"

#include <unistd.h>

std::map<int, std::size_t> ManagedFd::fd_count_ = std::map<int, std::size_t>();

ManagedFd::ManagedFd() : fd_(kNofd) {}

ManagedFd::ManagedFd(int fd) : fd_(fd)
{
	if (fd < 0) {
		return;
	}
	fd_count_[fd_]++;
}

ManagedFd::ManagedFd(const ManagedFd &copy) : fd_(copy.fd_)
{
	if (fd_ < 0) {
		return;
	}
	fd_count_[fd_]++;
}

ManagedFd &ManagedFd::operator=(const ManagedFd &other)
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

ManagedFd::~ManagedFd()
{
	if (fd_ < 0) {
		return;
	}
	fd_count_[fd_]--;
	if (fd_count_[fd_] == 0) {
		close(fd_);
	}
}

int ManagedFd::GetFd() const
{
	return fd_;
}
