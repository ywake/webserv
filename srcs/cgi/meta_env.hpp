#include "i_request.hpp"
#include <vector>

namespace cgi
{
	void SetAuthType(std::vector<char *> &args, const server::IRequest &request);
	void SetContentLength(std::vector<char *> &args, const server::IRequest &request);
	void SetContentType(std::vector<char *> &args, const server::IRequest &request);
} // namespace cgi
