#include "fdset.hpp"
#include "debug.hpp"

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
	std::size_t index = fd / kBits;
	if (index >= ready_fds_.capacity()) {
		ready_fds_.reserve(index * 2);
	}
	log("set index", index);
	log("set bit", fd % kBits);
	ready_fds_[index] |= 1 << (fd % kBits);
}

bool FdSet::IsReady(int fd)
{
	std::size_t index = fd / kBits;
	if (index >= ready_fds_.capacity()) {
		return false;
	}
	log("ready index", index);
	log("ready bit", fd % kBits);
	log("is ready", (bool)(ready_fds_[index] & (1 << (fd % kBits))));
	return ready_fds_[index] & (1 << (fd % kBits));
}

void FdSet::Reset()
{
	ready_fds_ = std::vector<uint64_t>(ready_fds_.size(), 0);
}