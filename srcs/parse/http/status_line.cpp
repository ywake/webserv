#include "status_line.hpp"

namespace http
{
	bool StatusLine::operator==(const StatusLine &rhs) const
	{
		return http_version_ == rhs.http_version_ && status_code_ == rhs.status_code_;
	}
} // namespace http
