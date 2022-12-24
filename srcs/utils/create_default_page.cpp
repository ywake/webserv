#include <string>

#include "status_code.hpp"

namespace utils
{
	std::string CreateDefaultPage(const http::StatusCode &code)
	{
		std::string title = code.GetCodeStr() + " " + code.GetReasonPhrase();
		std::string page  = "<html>\n"
							"<head><title>" +
						   title +
						   "</title></head>\n"
						   "<body>\n"
						   "<center><h1>" +
						   title +
						   "</h1></center>\n"
						   "<hr><center>webserv/1.0</center>\n"
						   "</body>\n"
						   "</html>\n";
		return page;
	}
} // namespace utils
