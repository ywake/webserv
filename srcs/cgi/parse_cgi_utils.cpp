#include "parse_cgi_utils.hpp"
#include "http_define.hpp"

namespace cgi
{
	ThinString TrimNewline(const ThinString &str)
	{
		static const std::string kNl = "\n";

		ThinString trimed = str.TrimRight(http::kCrLf);
		if (!trimed.empty()) {
			return trimed;
		}
		return str.TrimRight(kNl);
	}

} // namespace cgi
