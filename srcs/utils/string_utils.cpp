#include <string>

namespace utils
{
	char &GetLastChar(std::string str)
	{
		return str.at(str.size() - 1);
	}
} // namespace utils
