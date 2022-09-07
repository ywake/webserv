#ifndef URI_HPP
#define URI_HPP
#include "ThinString.hpp"
#include <iostream>
#include <string>

class Uri
{
  public:
	std::string scheme_;
	std::string userinfo_;
	std::string host_;
	std::string port_;
	std::string path_;
	std::string query_;
	std::string fragment_;

	Uri(std::string &scheme,
		std::string &userinfo,
		std::string &host,
		std::string &port,
		std::string &path,
		std::string &query,
		std::string &fragment);
	Uri(const char *scheme = "",
		const char *userinfo = "",
		const char *host = "",
		const char *port = "",
		const char *path = "",
		const char *query = "",
		const char *fragment = "");

	bool operator==(const Uri &rhs) const;

	static Uri ConstructFromOrigin(ThinString origin_form);
};

std::ostream &operator<<(std::ostream &os, const Uri &uri);

#endif
