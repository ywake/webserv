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
		return FileType::kRegularFile;
	} else if (IsDirectory()) {
		return FileType::kDirectory;
	} else if (IsCharacterDevice()) {
		return FileType::kCharacterDevice;
	} else if (IsBlockDevice()) {
		return FileType::kBlockDevice;
	} else if (IsFifo()) {
		return FileType::kFifo;
	} else {
		return FileType::kUnknown;
	}
}

std::size_t Stat::FileSize() const
{
	return stat_.st_size;
}
