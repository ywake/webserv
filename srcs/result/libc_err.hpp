#ifndef LIBC_ERR_HPP
#define LIBC_ERR_HPP

#include "result.hpp"

namespace err
{
	extern result::ErrCode kUnknownErr;

	namespace stat
	{
		extern result::ErrCode kEAcces;
		extern result::ErrCode kEBadFd;
		extern result::ErrCode kEFault;
		extern result::ErrCode kEIO;
		extern result::ErrCode kELoop;
		extern result::ErrCode kENameTooLong;
		extern result::ErrCode kNoEnt;
		extern result::ErrCode kENotDir;
		extern result::ErrCode kEOverFlow;
		extern result::ErrCode kENoMem;
		extern result::ErrCode kEInval;
		result::ErrCode        GetErrCode(int error_number);
	} // namespace stat

	namespace opendir
	{
		extern result::ErrCode kEAccess;
		extern result::ErrCode kEBadf;
		extern result::ErrCode kEMfile;
		extern result::ErrCode kENfile;
		extern result::ErrCode kENoEnt;
		extern result::ErrCode kENoMem;
		extern result::ErrCode kENotDir;
		result::ErrCode        GetErrCode(int error_number);
	} // namespace opendir
} // namespace err
#endif
