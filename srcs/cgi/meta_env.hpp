#include "i_request.hpp"
#include <vector>

namespace cgi
{
	void SetAuthType(std::vector<const char *> &envs, const server::IRequest &request);
	void SetContentLength(std::vector<const char *> &envs, const server::IRequest &request);
} // namespace cgi
