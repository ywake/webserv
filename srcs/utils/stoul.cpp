#include <cerrno>
#include <cstdlib>
#include <stdexcept>
#include <string>

namespace utils
{
	std::size_t stoul(const std::string &str, std::size_t *idx = NULL, unsigned long base = 10)
	{
		const char *p = str.c_str();
		char       *end;
		errno           = 0;
		unsigned long x = std::strtoul(p, &end, base);
		if (p == end) {
			throw std::invalid_argument("stoul");
		}
		if (errno == ERANGE) {
			throw std::out_of_range("stoul");
		}
		if (idx != NULL) {
			*idx = static_cast<std::size_t>(end - p);
		}
		return x;
	}
} // namespace utils
