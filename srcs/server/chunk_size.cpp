#include <cerrno>
#include <cstdio>
#include <cstdlib>

#include "http_define.hpp"
#include "parse_abnf_core_rules.hpp"
#include "result.hpp"

namespace http
{

	static bool HasValidCharsAfterHex(const std::string &s)
	{
		std::size_t i = 0;
		for (; i < s.size(); i++) {
			if (s[i] == ';') {
				break;
			}
			if (http::kWhiteSpaces.find(s[i]) == std::string::npos) {
				return false;
			}
		}
		for (; i < s.size(); i++) {
			if (!ABNF::IsVchar(s[i]) && http::kWhiteSpaces.find(s[i]) == std::string::npos) {
				return false;
			}
		}
		return true;
	}

	static const std::string ExtractHexStr(const std::string &s)
	{
		std::string hex;
		std::size_t i = 0;

		for (; i < s.size(); i++) {
			if (!ABNF::IsHexDigit(s[i])) {
				break;
			}
		}
		return s.substr(0, i);
	}

	// chunk-size [ chunk-ext ]
	Result<std::size_t> ParseChunkSize(const std::string &chunk_size)
	{
		const std::string hex = ExtractHexStr(chunk_size);
		if (hex.empty()) {
			return Error("chunk size empty");
		}
		if (!HasValidCharsAfterHex(chunk_size.substr(hex.size()))) {
			return Error("chunk size invalid char");
		}
		errno            = 0;
		std::size_t size = std::strtoul(hex.c_str(), NULL, 16);
		if (errno) {
			return Error("chunksize strtol: " + std::string(strerror(errno)));
		}
		return size;
	}

} // namespace http
