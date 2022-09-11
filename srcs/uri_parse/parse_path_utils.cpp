#include "parse_path_utils.hpp"

#include "parse_define.hpp"

namespace ABNF
{
	StringAry TokenizePath(const ThinString &str)
	{
		StringAry tokens;

		for (ThinString::const_iterator itr = str.begin(); itr != str.end();) {
			std::size_t start_index = itr - str.begin();
			std::size_t token_len	= 0;
			if (*itr == '/') {
				token_len = sizeof(char);
			} else {
				token_len = str.substr(start_index).MeasureUntil("/");
			}
			tokens.push_back(str.substr(start_index, token_len));
			itr += token_len;
		}
		return tokens;
	}

	// abcd%88ab%99a
	// a, b, c, d, %88, a, b, %99, a
	// TODO pchar以外の判定にも使ってるので関数名考える
	StringAry TokenizePchar(const ThinString &str)
	{
		StringAry tokens;
		for (std::string::const_iterator itr = str.begin(); itr != str.end();) {
			std::size_t token_start = itr - str.begin();
			std::size_t token_len	= *itr == '%' ? kPctEncodingSize : sizeof(char);
			tokens.push_back(str.substr(token_start, token_len));
			itr += tokens.back().size();
		}
		return tokens;
	}
} // namespace ABNF
