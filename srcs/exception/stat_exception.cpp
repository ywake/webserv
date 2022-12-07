#include "stat_exception.hpp"

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
		return StatError::kEAcces;
	case EBADF:
		return StatError::kEBadFd;
	case EFAULT:
		return StatError::kEFault;
	case EIO:
		return StatError::kEIO;
	case ELOOP:
		return StatError::kELoop;
	case ENAMETOOLONG:
		return StatError::kENameTooLong;
	case ENOENT:
		return StatError::kNoEnt;
	case ENOTDIR:
		return StatError::kENotDir;
	case EOVERFLOW:
		return StatError::kEOverFlow;
	case ENOMEM:
		return StatError::kENoMem;
	case EINVAL:
		return StatError::kEInval;
	default:
		return StatError::kUnknown;
	}
}
