#include "parse_cgi_utils.hpp"
#include "http_define.hpp"

namespace cgi
{
	ThinString TrimNewline(const ThinString &str)
	{
		ThinString trimed = str.TrimRight(http::kCrLf);
		if (!trimed.empty()) {
			return trimed;
		}
		return str.TrimRight(http::kNl);
	}

} // namespace cgi
