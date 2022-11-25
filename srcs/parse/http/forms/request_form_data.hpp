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
		std::string &scheme,
		std::string &userinfo,
		std::string &host,
		std::string &port,
		std::string &path,
		std::string &query
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

	static RequestFormData ConstructFromOrigin(ThinString origin_form);
};

std::ostream &operator<<(std::ostream &os, const RequestFormData &uri);

#endif
