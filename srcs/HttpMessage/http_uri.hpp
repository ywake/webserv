#ifndef HTTP_URI_HPP
#define HTTP_URI_HPP

#include "uri.hpp"

class HttpUri : public Uri
{
public:
	HttpUri();
	void ParseOriginForm(ThinString uri);
	void ParseAbsoluteForm(ThinString uri);
	void ParseAuthorityForm(ThinString uri);
};


#endif // HTTP_URI_HPP
