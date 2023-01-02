#include "i_request.hpp"

#include <map>
#include <string>
#include <vector>

namespace cgi
{
	typedef std::map<std::string, std::string> MetaEnvs;

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

	void SetContentLength(MetaEnvs &envs, const server::IRequest &request);
	void SetContentType(MetaEnvs &envs, const server::IRequest &request);
	void SetGatewayInterface(MetaEnvs &envs);
	void SetPathEnvs(
		MetaEnvs               &envs,
		const server::IRequest &request,
		const std::string      &root,
		const std::string      &script_name
	);
	void SetQueryString(MetaEnvs &envs, const server::IRequest &request);
	void SetRemoteAddr(MetaEnvs &envs, const std::string &ip);
	void SetRequestMethod(MetaEnvs &envs, const server::IRequest &request);
	void SetServerName(MetaEnvs &envs, const server::IRequest &request);
	void SetServerPort(MetaEnvs &envs, const std::string &port);
	void SetServerProtocol(MetaEnvs &envs);
	void SetServerSoftware(MetaEnvs &envs);
} // namespace cgi
