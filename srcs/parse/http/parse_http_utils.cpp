#include <string>
#include <vector>

#include "http_define.hpp"
#include "parse_http_utils.hpp"
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
		return TrimRightOws(TrimLeftOws(value));
	}

	ThinString TrimLeftOws(const ThinString &value)
	{
		return value.TrimLeft(kWhiteSpaces);
	}

	ThinString TrimRightOws(const ThinString &value)
	{
		std::size_t end_idx = value.RFindNotOf(kWhiteSpaces);
		if (end_idx == ThinString::npos) {
			return "";
		}
		ThinString trimed_ows          = value.substr(0, end_idx + 1);
		ThinString trimed_with_obsfold = value.substr(0, end_idx + 2);
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
			ThinString trimmed = *it;
			if (it != splitted.begin()) {
				trimmed = TrimLeftOws(*it);
			}
			if (it != splitted.end() - 1) {
				trimmed = TrimRightOws(trimmed);
			}
			parsed.push_back(trimmed);
		}

		return parsed;
	}

	Result<std::string> NormalizePath(const ThinString &path)
	{
		Result<std::string> res = utils::NormalizePath(ThinString(path));
		if (res.IsErr()) {
			return res.Err();
		}
		std::string &normalized = res.Val();
		if (normalized.empty()) {
			return std::string("/");
		}
		if (*normalized.begin() != '/') {
			return "/" + normalized;
		}
		return normalized;
	}
} // namespace http
