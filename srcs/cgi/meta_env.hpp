#include "i_request.hpp"
#include <vector>

namespace cgi
{
	extern const std::string kContentLength;
	extern const std::string kContentType;
	extern const std::string kGatewayInterface;
	extern const std::string kPathInfo;
	extern const std::string kPathTranslated;
	extern const std::string kQueryString;
	extern const std::string kRemoteAddr;
	extern const std::string kRequestMethod;
	extern const std::string kScriptName;
	extern const std::string kServerAddr;
	extern const std::string kServerPort;
	extern const std::string kServerProtocol;
	extern const std::string kServerSoftware;

	void SetAuthType(std::vector<const char *> &envs, const server::IRequest &request);
	void SetContentLength(std::vector<const char *> &envs, const server::IRequest &request);
	void SetContentType(std::vector<const char *> &envs, const server::IRequest &request);
} // namespace cgi
