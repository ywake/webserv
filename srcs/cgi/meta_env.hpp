#include "i_request.hpp"
#include <vector>

namespace cgi
{
	void SetAuthType(std::vector<const char *> &args, const server::IRequest &request);
} // namespace cgi
