#include <string>
#include <sys/stat.h>

#include "result.hpp"
#include "stat_exception.hpp"

class Stat
{
  private:
	const std::string path_;
	struct stat       stat_;

  public:
	// factory
	static Result<Stat> FromPath(const std::string &path);
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
