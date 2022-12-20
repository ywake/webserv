#include <cerrno>
#include <cstring>

#include "libc_err.hpp"
#include "result.hpp"

namespace err
{
	result::ErrCode kUnknownErr = new Error("stat: unknown error");

	namespace stat
	{
		result::ErrCode kEAcces       = new Error("stat: " + std::string(strerror(EACCES)));
		result::ErrCode kEBadFd       = new Error("stat: " + std::string(strerror(EBADF)));
		result::ErrCode kEFault       = new Error("stat: " + std::string(strerror(EFAULT)));
		result::ErrCode kEIO          = new Error("stat: " + std::string(strerror(EIO)));
		result::ErrCode kELoop        = new Error("stat: " + std::string(strerror(ELOOP)));
		result::ErrCode kENameTooLong = new Error("stat: " + std::string(strerror(ENAMETOOLONG)));
		result::ErrCode kNoEnt        = new Error("stat: " + std::string(strerror(ENOENT)));
		result::ErrCode kENotDir      = new Error("stat: " + std::string(strerror(ENOTDIR)));
		result::ErrCode kEOverFlow    = new Error("stat: " + std::string(strerror(EOVERFLOW)));
		result::ErrCode kENoMem       = new Error("stat: " + std::string(strerror(ENOMEM)));
		result::ErrCode kEInval       = new Error("stat: " + std::string(strerror(EINVAL)));

		result::ErrCode GetErrCode(int error_number)
		{
			switch (error_number) {
			case EACCES:
				return kEAcces;
			case EBADF:
				return kEBadFd;
			case EFAULT:
				return kEFault;
			case EIO:
				return kEIO;
			case ELOOP:
				return kELoop;
			case ENAMETOOLONG:
				return kENameTooLong;
			case ENOENT:
				return kNoEnt;
			case ENOTDIR:
				return kENotDir;
			case EOVERFLOW:
				return kEOverFlow;
			case ENOMEM:
				return kENoMem;
			case EINVAL:
				return kEInval;
			default:
				return kUnknownErr;
			}
		}
	} // namespace stat

	namespace opendir
	{
		result::ErrCode kEAccess = new Error("opendir: " + std::string(strerror(EACCES)));
		result::ErrCode kEBadf   = new Error("opendir: " + std::string(strerror(EBADF)));
		result::ErrCode kEMfile  = new Error("opendir: " + std::string(strerror(EMFILE)));
		result::ErrCode kENfile  = new Error("opendir: " + std::string(strerror(ENFILE)));
		result::ErrCode kENoEnt  = new Error("opendir: " + std::string(strerror(ENOENT)));
		result::ErrCode kENoMem  = new Error("opendir: " + std::string(strerror(ENOMEM)));
		result::ErrCode kENotDir = new Error("opendir: " + std::string(strerror(ENOTDIR)));

		result::ErrCode GetErrCode(int error_number)
		{
			switch (error_number) {
			case EACCES:
				return kEAccess;
			case EBADF:
				return kEBadf;
			case EMFILE:
				return kEMfile;
			case ENFILE:
				return kENfile;
			case ENOENT:
				return kENoEnt;
			case ENOMEM:
				return kENoMem;
			case ENOTDIR:
				return kENotDir;
			default:
				return kUnknownErr;
			}
		}
	} // namespace opendir
} // namespace err
