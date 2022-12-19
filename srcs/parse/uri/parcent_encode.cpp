#include <sstream>
#include <string>

#include "parse_abnf_core_rules.hpp"

namespace uri
{
	static const std::string kNeedToEncodes = " !&#$%\"'()*+,:;<=>?@[\\]^`{|}";

	std::string PercentEncode(const std::string &s)
	{
		std::stringstream ss;

		for (std::size_t i = 0; i < s.size(); i++) {
			unsigned char c = s[i];
			if (!ABNF::IsVchar(c) || kNeedToEncodes.find(c) != std::string::npos) {
				ss << "%" << std::hex << std::uppercase << (size_t)c;
				continue;
			}
			ss << c;
		}
		return ss.str();
	}

} // namespace uri
