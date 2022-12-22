#include <cstring>
#include <sstream>
#include <string>

#include "parse_abnf_core_rules.hpp"
#include "parse_define.hpp"
#include "parse_uri_relative.hpp"
#include "result.hpp"
#include "thin_string.hpp"

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

	Result<unsigned char> DecodePctStr(const ThinString &pct_str)
	{
		if (!ABNF::IsPctEncoded(pct_str) || pct_str == "%00") {
			return Error("pct decode error");
		}
		std::string pct_encoded = pct_str.ToString();
		const char *hex         = pct_encoded.c_str() + sizeof('%');
		return strtoul(hex, NULL, 16);
	}

	Result<std::string> PercentDecode(const ThinString &s)
	{
		std::string decoded;

		for (std::size_t i = 0; i < s.size();) {
			if (s.at(i) != '%') {
				decoded.push_back(s.at(i));
				i++;
				continue;
			}
			Result<unsigned char> c = DecodePctStr(s.substr(i, ABNF::kPctEncodingSize));
			if (c.IsErr()) {
				return c.Err();
			}
			decoded.push_back(c.Val());
			i += ABNF::kPctEncodingSize;
		}
		return decoded;
	}

} // namespace uri
