#include "result.hpp"
#include <fstream>
#include <string>

namespace utils
{

	Result<std::string> ReadFile(const std::string &file_path)
	{
		std::ifstream ifs(file_path.c_str());
		if (!ifs) {
			return Result<std::string>(Error("file open error"));
		}

		std::string line, file_content;
		while (std::getline(ifs, line)) {
			file_content += line + "\n";
		}

		return Result<std::string>(file_content);
	}
} // namespace utils
