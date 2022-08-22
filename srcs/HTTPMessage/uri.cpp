#include "uri.hpp"

URI::URI() : scheme_(), userinfo_(), host_(), port_(), path_(), query_(), fragment_() {}

URI::URI(
	std::string &scheme,
	std::string &userinfo,
	std::string &host,
	std::string &port,
	std::string &path,
	std::string &query,
	std::string &fragment
)
	: scheme_(scheme), userinfo_(userinfo), host_(host), port_(port), path_(path), query_(query),
	  fragment_(fragment)
{
}

URI::URI(
	const char *scheme,
	const char *userinfo,
	const char *host,
	const char *port,
	const char *path,
	const char *query,
	const char *fragment
)
	: scheme_(scheme), userinfo_(userinfo), host_(host), port_(port), path_(path), query_(query),
	  fragment_(fragment)
{
}

bool URI::operator==(const URI &rhs) const
{
	return scheme_ == rhs.scheme_ && userinfo_ == rhs.userinfo_ && host_ == rhs.host_ &&
		   port_ == rhs.port_ && path_ == rhs.path_ && query_ == rhs.query_ &&
		   fragment_ == rhs.fragment_;
}

std::ostream &operator<<(std::ostream &os, const URI &uri)
{
	os << "scheme   : " << uri.scheme_ << std::endl;
	os << "userinfo : " << uri.userinfo_ << std::endl;
	os << "host     : " << uri.host_ << std::endl;
	os << "port     : " << uri.port_ << std::endl;
	os << "path     : " << uri.path_ << std::endl;
	os << "query    : " << uri.query_ << std::endl;
	os << "fragment : " << uri.fragment_ << std::endl;
	return os;
}
