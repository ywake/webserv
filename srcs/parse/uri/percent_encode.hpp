#ifndef PERCENT_ENCODE_HPP
#define PERCENT_ENCODE_HPP

#include <string>

#include "result.hpp"
#include "thin_string.hpp"

namespace uri
{
	std::string         PercentEncode(const std::string &s);
	Result<std::string> PercentDecode(const ThinString &s);
} // namespace uri

#endif
