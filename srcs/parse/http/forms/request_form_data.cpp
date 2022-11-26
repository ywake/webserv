#include "request_form_data.hpp"

RequestFormData::RequestFormData(
	const std::string &scheme,
	const std::string &userinfo,
	const std::string &host,
	const std::string &port,
	const std::string &path,
	const std::string &query
)
	: scheme_(scheme), userinfo_(userinfo), host_(host), port_(port), path_(path), query_(query)
{}

RequestFormData::RequestFormData(
	const char *scheme,
	const char *userinfo,
	const char *host,
	const char *port,
	const char *path,
	const char *query
)
	: scheme_(scheme), userinfo_(userinfo), host_(host), port_(port), path_(path), query_(query)
{}

RequestFormData &RequestFormData::operator=(const RequestFormData &other)
{
	if (this == &other) {
		return *this;
	}
	scheme_   = other.scheme_;
	userinfo_ = other.userinfo_;
	host_     = other.host_;
	port_     = other.port_;
	path_     = other.path_;
	query_    = other.query_;
	return *this;
}

bool RequestFormData::operator==(const RequestFormData &rhs) const
{
	if (this == &rhs) {
		return true;
	}
	return scheme_ == rhs.scheme_ && userinfo_ == rhs.userinfo_ && host_ == rhs.host_ &&
		   port_ == rhs.port_ && path_ == rhs.path_ && query_ == rhs.query_;
}

std::ostream &operator<<(std::ostream &os, const RequestFormData &uri)
{
	os << "scheme   : " << uri.scheme_ << std::endl;
	os << "userinfo : " << uri.userinfo_ << std::endl;
	os << "host     : " << uri.host_ << std::endl;
	os << "port     : " << uri.port_ << std::endl;
	os << "path     : " << uri.path_ << std::endl;
	os << "query    : " << uri.query_ << std::endl;
	return os;
}
