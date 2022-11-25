#ifndef REQUEST_FORM_DATA_HPP
#define REQUEST_FORM_DATA_HPP
#include "thin_string.hpp"
#include <iostream>
#include <string>

class RequestFormData
{
  public:
	std::string scheme_;
	std::string userinfo_;
	std::string host_;
	std::string port_;
	std::string path_;
	std::string query_;

	RequestFormData(
		const std::string &scheme,
		const std::string &userinfo,
		const std::string &host,
		const std::string &port,
		const std::string &path,
		const std::string &query
	);

	RequestFormData(
		const char *scheme   = "",
		const char *userinfo = "",
		const char *host     = "",
		const char *port     = "",
		const char *path     = "",
		const char *query    = ""
	);

	RequestFormData &operator=(const RequestFormData &other);
	bool             operator==(const RequestFormData &rhs) const;
};

std::ostream &operator<<(std::ostream &os, const RequestFormData &uri);

#endif
