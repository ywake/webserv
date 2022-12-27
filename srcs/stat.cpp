#include "stat.hpp"
#include <cerrno>
#include <unistd.h>

const result::ErrCode Stat::kEAcces = new Error("stat: " + std::string(strerror(EACCES)));
const result::ErrCode Stat::kEBadFd = new Error("stat: " + std::string(strerror(EBADF)));
const result::ErrCode Stat::kEFault = new Error("stat: " + std::string(strerror(EFAULT)));
const result::ErrCode Stat::kEIO    = new Error("stat: " + std::string(strerror(EIO)));
const result::ErrCode Stat::kELoop  = new Error("stat: " + std::string(strerror(ELOOP)));
const result::ErrCode Stat::kENameTooLong =
	new Error("stat: " + std::string(strerror(ENAMETOOLONG)));
const result::ErrCode Stat::kNoEnt      = new Error("stat: " + std::string(strerror(ENOENT)));
const result::ErrCode Stat::kENotDir    = new Error("stat: " + std::string(strerror(ENOTDIR)));
const result::ErrCode Stat::kEOverFlow  = new Error("stat: " + std::string(strerror(EOVERFLOW)));
const result::ErrCode Stat::kENoMem     = new Error("stat: " + std::string(strerror(ENOMEM)));
const result::ErrCode Stat::kEInval     = new Error("stat: " + std::string(strerror(EINVAL)));
const result::ErrCode Stat::kUnknownErr = new Error("stat: unknown error");

result::ErrCode Stat::GetErrCode(int error_number)
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

result::Result<Stat> Stat::FromPath(const std::string &path)
{
	struct stat stat_buf;
	int         res;

	res = stat(path.c_str(), &stat_buf);
	if (res == -1) {
		return GetErrCode(errno);
	}
	return Stat(path, stat_buf);
}

Stat::Stat() : path_(), stat_() {}

Stat::Stat(const std::string &path, const struct stat &stat) : path_(path), stat_(stat) {}

Stat::~Stat() {}

bool Stat::IsRegularFile() const
{
	return S_ISREG(stat_.st_mode);
}

bool Stat::IsDirectory() const
{
	return S_ISDIR(stat_.st_mode);
}

bool Stat::IsCharacterDevice() const
{
	return S_ISCHR(stat_.st_mode);
}

bool Stat::IsBlockDevice() const
{
	return S_ISBLK(stat_.st_mode);
}

bool Stat::IsFifo() const
{
	return S_ISFIFO(stat_.st_mode);
}

Stat::FileType Stat::GetFileType() const
{
	if (IsRegularFile()) {
		return Stat::kRegularFile;
	} else if (IsDirectory()) {
		return Stat::kDirectory;
	} else if (IsCharacterDevice()) {
		return Stat::kCharacterDevice;
	} else if (IsBlockDevice()) {
		return Stat::kBlockDevice;
	} else if (IsFifo()) {
		return Stat::kFifo;
	} else {
		return Stat::kUnknown;
	}
}

std::size_t Stat::FileSize() const
{
	return stat_.st_size;
}
