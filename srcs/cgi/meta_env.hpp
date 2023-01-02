#include "i_request.hpp"

#include <map>
#include <string>
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
	extern const std::string kServerName;
	extern const std::string kServerPort;
	extern const std::string kServerProtocol;
	extern const std::string kServerSoftware;

	void SetContentLength(std::vector<std::string> &envs, const server::IRequest &request);
	void SetContentType(std::vector<std::string> &envs, const server::IRequest &request);
	void SetGatewayInterface(std::vector<std::string> &envs);
	void SetPathEnvs(
		std::vector<std::string> &envs,
		const server::IRequest   &request,
		const std::string        &root,
		const std::string        &script_name
	);
	void SetQueryString(std::vector<std::string> &envs, const server::IRequest &request);
	void SetRemoteAddr(std::vector<std::string> &envs, const std::string &ip);
	void SetRequestMethod(std::vector<std::string> &envs, const server::IRequest &request);
	void SetServerName(std::vector<std::string> &envs, const server::IRequest &request);
	void SetServerPort(std::vector<std::string> &envs, const std::string &port);
	void SetServerProtocol(std::vector<std::string> &envs);
	void SetServerSoftware(std::vector<std::string> &envs);
} // namespace cgi
