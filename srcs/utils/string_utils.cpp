#include <string>
#include <vector>

namespace utils
{
	char &GetLastChar(std::string str)
	{
		return str.at(str.size() - 1);
	}

	std::pair<std::string, std::string> DivideBy(std::string str, char c)
	{
		std::string left;
		std::string right;
		size_t      pos = str.find(c);
		if (pos == std::string::npos) {
			left  = str;
			right = "";
			return std::pair<std::string, std::string>(left, right);
		}
		/* pos != std::string::npos */
		left = str.substr(0, pos);
		if (pos == str.size() - 1) {
			right = "";
		} else {
			right = str.substr(pos + 1);
		}
		return std::pair<std::string, std::string>(left, right);
	}

	std::string Join(const std::vector<std::string> &str_array, const std::string &separator)
	{
		std::string joined_str;
		for (size_t i = 0; i < str_array.size(); ++i) {
			if (i != 0) {
				joined_str += separator;
			}
			joined_str += str_array[i];
		}
		return joined_str;
	}
} // namespace utils
