#include "i_request.hpp"
#include <vector>

namespace cgi
{
	void SetAuthType(std::vector<char *> &args, const server::IRequest &request);
	void SetContentLength(std::vector<char *> &args, const server::IRequest &request);
	void SetContentType(std::vector<char *> &args, const server::IRequest &request);
	void SetPathInfo(std::vector<const char *> &args, const std::string &path_info);
} // namespace cgi
