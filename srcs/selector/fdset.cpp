#include "fdset.hpp"

FdSet::FdSet()
	: ready_fds_(1024) {}
FdSet::FdSet(const FdSet &copy)
	: ready_fds_(copy.ready_fds_) {}
const FdSet &FdSet::operator=(const FdSet &other)
{
	if (this == &other) {
		return *this;
	}
	ready_fds_ = other.ready_fds_;
	return *this;
}
FdSet::~FdSet() {}

void FdSet::SetFd(int fd)
{
	std::size_t index = fd / sizeof(uint64_t);
	if (index >= ready_fds_.capacity()) {
		ready_fds_.reserve(index * 2);
	}
	ready_fds_[index] |= 1 << (fd % sizeof(uint64_t));
}

bool FdSet::IsReady(int fd)
{
	std::size_t index = fd / sizeof(uint64_t);
	if (index >= ready_fds_.capacity()) {
		return false;
	}
	return ready_fds_[index] & (1 << (fd % sizeof(uint64_t)));
}