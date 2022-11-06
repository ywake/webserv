#include <cerrno>
#include <cstdlib>
#include <stdexcept>
#include <string>

#include "result.hpp"

namespace utils
{
	Result<std::size_t> StrToUnsignedLong(const std::string &str)
	{
		char *end = NULL;
		errno     = 0;
		size_t n  = std::strtoul(str.c_str(), &end, 10);

		bool is_empty   = str.c_str() == end;
		bool is_all_num = *end == '\0';
		if (is_empty || !is_all_num || errno) {
			return Error("StrToUnsignedLong error");
		}

		return n;
	}
} // namespace utils
