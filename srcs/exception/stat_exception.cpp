#include "stat_exception.hpp"

#include <cerrno>
#include <cstring>

StatException::StatException(int error_number) : error_number_(error_number) {}

const char *StatException::what() const throw()
{
	return strerror(error_number_);
}

StatException::StatError StatException::error() const
{
	switch (error_number_) {
	case EACCES:
		return StatException::kEAcces;
	case EBADF:
		return StatException::kEBadFd;
	case EFAULT:
		return StatException::kEFault;
	case EIO:
		return StatException::kEIO;
	case ELOOP:
		return StatException::kELoop;
	case ENAMETOOLONG:
		return StatException::kENameTooLong;
	case ENOENT:
		return StatException::kNoEnt;
	case ENOTDIR:
		return StatException::kENotDir;
	case EOVERFLOW:
		return StatException::kEOverFlow;
	case ENOMEM:
		return StatException::kENoMem;
	case EINVAL:
		return StatException::kEInval;
	default:
		return StatException::kUnknown;
	}
}
