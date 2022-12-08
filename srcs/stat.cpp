#include "stat.hpp"
#include <cerrno>
#include <unistd.h>

Result<Stat> Stat::FromPath(const std::string &path)
{
	struct stat stat_buf;
	int         res;

	res = stat(path.c_str(), &stat_buf);
	if (res == -1) {
		return Error(errno);
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
