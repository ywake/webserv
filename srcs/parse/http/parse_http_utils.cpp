#include <string>
#include <vector>

#include "http_define.hpp"
#include "thin_string.hpp"
#include "validate_field_line.hpp"
#include "webserv_utils.hpp"

std::vector<ThinString> TokenizeList(const ThinString &str)
{
	std::vector<ThinString> tokens;

	bool   in_quote  = false;
	size_t start_idx = 0, end_idx = 0;
	for (size_t i = 0; i < str.size(); ++i, end_idx = i) {
		if (str.at(i) == '"') {
			in_quote = !in_quote;
		}
		if (!in_quote && str.at(i) == ',') {
			tokens.push_back(str.substr(start_idx, end_idx - start_idx));
			start_idx = i + 1;
		}
	}
	tokens.push_back(str.substr(start_idx, end_idx - start_idx));

	return tokens;
}

namespace http
{
	ThinString TrimOws(const ThinString &value)
	{
		ThinString  trimed_left = value.TrimLeft(kWhiteSpaces);
		std::size_t end_idx     = trimed_left.RFindNotOf(kWhiteSpaces);
		if (end_idx == ThinString::npos) {
			return "";
		}
		ThinString trimed_ows          = trimed_left.substr(0, end_idx + 1);
		ThinString trimed_with_obsfold = trimed_left.substr(0, end_idx + 2);
		if (abnf::EndWithObsFold(trimed_with_obsfold)) {
			return trimed_with_obsfold;
		} else {
			return trimed_ows;
		}
	}

	std::vector<ThinString> ParseList(const ThinString &str, bool ignore_quote)
	{
		typedef std::vector<ThinString> List;

		List splitted;
		if (ignore_quote) {
			splitted = Split(str, ",");
		} else {
			splitted = TokenizeList(str);
		}
		List parsed;
		for (List::const_iterator it = splitted.begin(); it != splitted.end(); ++it) {
			ThinString trimmed = TrimOws(*it);
			parsed.push_back(trimmed);
		}

		return parsed;
	}

} // namespace http
