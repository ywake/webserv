#ifndef PARSE_DEFINE_HPP
#define PARSE_DEFINE_HPP

#include "thin_string.hpp"
#include <vector>

namespace ABNF
{
	typedef std::vector<ThinString>			  StringAry;
	typedef std::pair<StringAry, StringAry>	  Ipv6TokensPair;
	typedef StringAry::const_iterator		  Iterator;	   // TODO rename
	typedef StringAry::const_reverse_iterator RevIterator; // TODO rename

	static const size_t kPctEncodingSize = 3;

} // namespace ABNF

#endif
