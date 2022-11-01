#include "webserv_utils.hpp"

#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

bool IsRegularFile(const std::string &path)
{
	struct stat st;

	if (stat(path.c_str(), &st) != 0) {
		return false;
	}

	return S_ISREG(st.st_mode);
}
