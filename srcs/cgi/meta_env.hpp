#include "i_request.hpp"
#include <vector>

namespace cgi
{
	void SetAuthType(std::vector<const char *> &envs, const server::IRequest &request);
	void SetContentLength(std::vector<const char *> &envs, const server::IRequest &request);
	void SetContentType(std::vector<const char *> &envs, const server::IRequest &request);
	void SetGatewayInterface(std::vector<const char *> &envs);
	void SetPathInfo(std::vector<const char *> &args, const std::string &path_info);
	void SetPathTranslated(
		std::vector<const char *> &envs,
		const std::string         &script_path,
		const std::string         &path_info
	);
	void SetQueryString(std::vector<const char *> &envs, const server::IRequest &request);
} // namespace cgi
