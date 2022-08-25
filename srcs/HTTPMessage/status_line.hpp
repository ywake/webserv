#ifndef STATUS_LINE_HPP
#define STATUS_LINE_HPP
#include <string>

class StatusLine
{
  public:
	std::string http_version_;
	std::string status_code_;
	// std::string reason_phrase_;

	// std::string Str()
	// {
	// return http_version_ + SP + status_code_ +
	// SP + reason_phrase_;
	// }
	bool operator==(const StatusLine &rhs) const
	{
		return http_version_ == rhs.http_version_ && status_code_ == rhs.status_code_;
	}
};

#endif
