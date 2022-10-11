#ifndef REQUEST_FORM_DATA_HPP
#define REQUEST_FORM_DATA_HPP
#include "thin_string.hpp"
#include <iostream>
#include <string>

// TODO std::stringに変更かも
class RequestFormData
{
  public:
	ThinString scheme_;
	ThinString userinfo_;
	ThinString host_;
	ThinString port_;
	ThinString path_;
	ThinString query_;

	RequestFormData(
		ThinString &scheme,
		ThinString &userinfo,
		ThinString &host,
		ThinString &port,
		ThinString &path,
		ThinString &query
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
