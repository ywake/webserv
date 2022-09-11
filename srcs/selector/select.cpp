#include <cerrno>

#include "select.hpp"

Select::Select() : nfds_()
{
	FD_ZERO(&read_set_);
}

Select::~Select() {}

Select::Select(Select const &copy) : nfds_(copy.nfds_), read_set_(copy.read_set_) {}

//[TODO]
Select &Select::operator=(Select const &other)
{
	if (this == &other) {
		return *this;
	}
	read_set_  = other.read_set_;
	ready_set_ = other.ready_set_;
	nfds_	   = other.nfds_;
	return *this;
}

Result<void> Select::Import(iterator begin, iterator end)
{
	FD_ZERO(&read_set_);
	for (iterator it = begin; it != end; ++it) {
		int fd = it->fd_;
		FD_SET(fd, &read_set_);
		nfds_ = std::max(nfds_, fd + 1);
	}
	return Result<void>();
}

Result<void> Select::Run()
{
	ready_set_ = read_set_;
	log("connect fd", FD_ISSET(5, &ready_set_));
	log("listen  fd", FD_ISSET(3, &ready_set_));
	if (select(nfds_, &ready_set_, NULL, NULL, NULL) < 0) {
		return Result<void>(Error(errno));
	}
	return Result<void>();
}

void Select::Export(FdSet &ready)
{
	// for (int i = 0; i < nfds_; i++) {
	// 	printf("%d\n", FD_ISSET(i, ready));
	// }
	// printf("\n");
	for (int i = 0; i < nfds_; i++) {
		if (FD_ISSET(i, &ready_set_)) {
			log("FD_ISSET", i);
			ready.SetFd(i);
		}
	}
	// memcpy(ready, &read_set_, sizeof(fd_set));
	//  for (int i = 0; i < nfds_; i++) {
	//  	printf("%d\n", FD_ISSET(i, ready));
	//  }
	//  printf("\n");
}
