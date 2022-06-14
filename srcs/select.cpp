#include <cerrno>

#include "select.hpp"

Select::Select()
	: nfds_()
{
	FD_ZERO(&read_set_);
}

Select::~Select() {}

Select::Select(Select const &copy)
	: nfds_(copy.nfds_), read_set_(copy.read_set_), ready_(copy.ready_) {}

//[TODO]
Select &Select::operator=(Select const &other)
{
	if (this == &other) {
		return *this;
	}
	// other.ready_ = ready_;
	return *this;
}

Result<void> Select::Import(iterator begin, iterator end)
{
	FD_ZERO(&read_set_);
	for (iterator it = begin; it != end; ++it) {
		int fd = it->first.GetFd();
		FD_SET(fd, &read_set_);
		nfds_ = std::max(nfds_, fd + 1);
	}
	return Result<void>();
}

Result<void> Select::Run()
{
	ready_set_ = read_set_;
	if (select(nfds_, &ready_set_, NULL, NULL, NULL) < 0) {
		return Result<void>(Error(errno));
	}
	return Result<void>();
}

const std::vector<int> &Select::Export()
{
	for (int i = 0; i < nfds_; i++) {
		if (FD_ISSET(i, &ready_set_)) {
			ready_.push_back(i);
		}
		// if (FD_ISSET(fd, &set->write_set)) {
		// 	event.Run();
		// }
	}
	return ready_;
}