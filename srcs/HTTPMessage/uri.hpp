#ifndef URI_HPP
#define URI_HPP
#include "ThinString.hpp"
#include <iostream>
#include <string>

class URI
{
  public:
	std::string scheme_;
	std::string userinfo_;
	std::string host_;
	std::string port_;
	std::string path_;
	std::string query_;
	std::string fragment_;

	URI(std::string &scheme,
		std::string &userinfo,
		std::string &host,
		std::string &port,
		std::string &path,
		std::string &query,
		std::string &fragment);
	URI(const char *scheme = "",
		const char *userinfo = "",
		const char *host = "",
		const char *port = "",
		const char *path = "",
		const char *query = "",
		const char *fragment = "");

	bool operator==(const URI &rhs) const;

	static URI ConstructFromOrigin(ThinString origin_form);
};

std::ostream &operator<<(std::ostream &os, const URI &uri);

#endif
