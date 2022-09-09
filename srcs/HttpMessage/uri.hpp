#ifndef URI_HPP
#define URI_HPP
#include "ThinString.hpp"
#include <iostream>
#include <string>

class Uri
{
  public:
	ThinString scheme_;
	ThinString userinfo_;
	ThinString host_;
	ThinString port_;
	ThinString path_;
	ThinString query_;

	Uri(ThinString &scheme,
		ThinString &userinfo,
		ThinString &host,
		ThinString &port,
		ThinString &path,
		ThinString &query);

	Uri(const char *scheme = "",
		const char *userinfo = "",
		const char *host = "",
		const char *port = "",
		const char *path = "",
		const char *query = "");

	bool operator==(const Uri &rhs) const;

	static Uri ConstructFromOrigin(ThinString origin_form);
};

std::ostream &operator<<(std::ostream &os, const Uri &uri);

#endif
