#ifndef STAT_HPP
#define STAT_HPP

#include <string>
#include <sys/stat.h>

#include "result.hpp"

class Stat
{
  public:
	static const result::ErrCode kEAcces;
	static const result::ErrCode kEBadFd;
	static const result::ErrCode kEFault;
	static const result::ErrCode kEIO;
	static const result::ErrCode kELoop;
	static const result::ErrCode kENameTooLong;
	static const result::ErrCode kNoEnt;
	static const result::ErrCode kENotDir;
	static const result::ErrCode kEOverFlow;
	static const result::ErrCode kENoMem;
	static const result::ErrCode kEInval;
	static const result::ErrCode kUnknownErr;

  private:
	const std::string      path_;
	struct stat            stat_;
	static result::ErrCode GetErrCode(int error_number_);

  public:
	// factory
	static result::Result<Stat> FromPath(const std::string &path);
	// constructor
	Stat();
	Stat(const std::string &path, const struct stat &stat);
	// FileCond(const std::string &path);
	~Stat();

	enum FileType {
		kRegularFile,
		kDirectory,
		kCharacterDevice,
		kBlockDevice,
		kFifo,
		kUnknown
	};

	// Getters
	bool     IsRegularFile() const;
	bool     IsDirectory() const;
	bool     IsCharacterDevice() const;
	bool     IsBlockDevice() const;
	bool     IsFifo() const;
	FileType GetFileType() const;

	std::size_t FileSize() const;
};
#endif
