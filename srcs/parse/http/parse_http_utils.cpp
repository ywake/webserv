#include <string>
#include <vector>

#include "http_define.hpp"
#include "thin_string.hpp"
#include "validate_field_line.hpp"
#include "webserv_utils.hpp"

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

	std::vector<ThinString> ParseList(const ThinString &str)
	{
		typedef std::vector<ThinString> List;

		List splitted = Split(str, ",");
		List parsed;

		for (List::const_iterator it = splitted.begin(); it != splitted.end(); ++it) {
			ThinString trimmed = TrimOws(*it);
			parsed.push_back(trimmed);
		}

		return parsed;
	}
} // namespace http
