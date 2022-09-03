#include "parse_ip_utils.hpp"

#include "parse_define.hpp"
#include "parse_ip.hpp"

namespace ABNF
{
	static const std::size_t kH16Bytes = 2;
	static const std::size_t kIpv4Bytes = 4;
	static const std::size_t kIpv6BytesMax = 16;

	bool HasMultiDcolon(const StringAry &tokens)
	{
		std::size_t num_of_dcolon = 0;
		for (StringAry::const_iterator itr = tokens.begin(); itr != tokens.end(); itr++) {
			if (*itr == "::") {
				num_of_dcolon++;
			}
			if (num_of_dcolon > 1) {
				return true;
			}
		}
		return false;
	}

	std::size_t CountDcolons(const StringAry &tokens)
	{
		std::size_t num_of_dcolon = 0;
		for (StringAry::const_iterator itr = tokens.begin(); itr != tokens.end(); itr++) {
			if (*itr == "::") {
				num_of_dcolon++;
			}
		}
		return num_of_dcolon;
	}

	bool IsValidColonPosition(StringAry &tokens)
	{
		if (tokens.empty()) {
			return true;
		}
		return tokens.front() != ":" && tokens.back() != ":";
	}

	Ipv6TokensPair DivideByDcolon(const StringAry &tokens)
	{
		typedef StringAry::const_iterator Iterator;
		Iterator itr = std::find<Iterator>(tokens.begin(), tokens.end(), "::");
		if (itr == tokens.end()) {
			return Ipv6TokensPair(StringAry(), tokens);
		}
		StringAry left = StringAry(tokens.begin(), itr);
		StringAry right = StringAry(itr + 1, tokens.end());
		return Ipv6TokensPair(left, right);
	}

	Result<std::size_t> CountH16Bytes(RevIterator begin, RevIterator end)
	{
		std::size_t bytes = 0;
		for (RevIterator itr = begin; itr != end; itr++) {
			if (*itr == ":") {
				continue;
			}
			if (!IsH16(*itr)) {
				return Error("");
			}
			bytes += kH16Bytes;
		}
		return bytes;
	}

	Result<std::size_t> CountLeftBytes(const StringAry &tokens)
	{
		return CountH16Bytes(tokens.rbegin(), tokens.rend());
	}

	Result<std::size_t> CountRightBytes(const StringAry &tokens)
	{
		if (tokens.empty()) {
			return 0;
		}
		Result<std::size_t> bytes = CountH16Bytes(++tokens.rbegin(), tokens.rend());
		if (bytes.IsErr()) {
			return Error("");
		} else if (IsIPv4address(tokens.back())) {
			return bytes.Val() + kIpv4Bytes;
		} else if (IsH16(tokens.back())) {
			return bytes.Val() + kH16Bytes;
		} else {
			return Error("");
		}
	}

	Result<std::size_t> CountIpv6Bytes(const StringAry &tokens)
	{
		Ipv6TokensPair left_right = DivideByDcolon(tokens);
		if (!IsValidColonPosition(left_right.first) || !IsValidColonPosition(left_right.second)) {
			return Error("");
		}
		Result<std::size_t> left_bytes = CountLeftBytes(left_right.first);
		if (left_bytes.IsErr()) {
			return Error("");
		}
		Result<std::size_t> right_bytes = CountRightBytes(left_right.second);
		if (right_bytes.IsErr()) {
			return Error("");
		}
		return left_bytes.Val() + right_bytes.Val();
	}

	// TODO refactor
	StringAry TokenizeIPv6address(const ThinString &str)
	{
		StringAry tokens;
		ThinString head = str;

		while (1) {
			std::size_t colon_index = head.MeasureUntil(":");
			std::size_t dcolon_index = head.MeasureUntil("::");
			std::size_t delim_len = 1 + (dcolon_index == colon_index);
			if (colon_index != 0) {
				tokens.push_back(head.substr(0, colon_index));
			}
			bool is_end = colon_index == head.len();
			if (is_end) {
				break;
			}
			tokens.push_back(head.substr(colon_index, delim_len));
			head = head.substr(colon_index + delim_len);
		}
		return tokens;
	}
} // namespace ABNF
