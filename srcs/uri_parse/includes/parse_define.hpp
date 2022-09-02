#ifndef PARSE_DEFINE_HPP
#define PARSE_DEFINE_HPP

#include "ThinString.hpp"
#include <vector>

namespace ABNF
{
	typedef std::vector<ThinString> StringAry;
	typedef std::pair<StringAry, StringAry> Ipv6TokensPair;
	typedef StringAry::const_iterator Iterator; // TODO rename

	static const size_t kPctEncodingSize = 3;

} // namespace ABNF

#endif
