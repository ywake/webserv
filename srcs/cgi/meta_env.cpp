#include "meta_env.hpp"
#include "cgi_response.hpp"
#include "i_request.hpp"
#include "webserv_utils.hpp"

#include <sstream>
#include <vector>

namespace cgi
{
	const std::string kContentLength    = "CONTENT_LENGTH";
	const std::string kContentType      = "CONTENT_TYPE";
	const std::string kGatewayInterface = "GATEWAY_INTERFACE";
	const std::string kPathInfo         = "PATH_INFO";
	const std::string kPathTranslated   = "PATH_TRANSLATED";
	const std::string kQueryString      = "QUERY_STRING";
	const std::string kRemoteAddr       = "REMOTE_ADDR";
	const std::string kRequestMethod    = "REQUEST_METHOD";
	const std::string kScriptName       = "SCRIPT_NAME";
	const std::string kServerAddr       = "SERVER_ADDR";
	const std::string kServerPort       = "SERVER_PORT";
	const std::string kServerProtocol   = "SERVER_PROTOCOL";
	const std::string kServerSoftware   = "SERVER_SOFTWARE";

	/**
	 * message-bodyの大きさをオクテット単位で10進数で設定。なければ未設定
	 * 転送符号化や内容符号化を除去した後の長さ
	 */
	void SetContentLength(MetaEnvs &envs, const server::IRequest &request)
	{
		if (!request.HasMessageBody()) {
			return;
		}
		envs[cgi::kContentLength] = request.Headers()[http::kContentLength].front().GetValue();
	}

	/**
	 * 鯖は HTTP Content-Type欄がクライアント要求頭に存在していれば
	 * 好めた変数を設定しなければ**なりません**。
	 * 鯖が実体は付されているにもかかわらず Content-Type
	 * 頭欄のない要求を受信したら、正しい内容型を決定しようと試みて構いませんが、
	 * そうでなければこのメタ変数は省略するべきです。
	 *
	 * Content-Type = media-type
	 * media-type = type "/" subtype *( ";" parameter )
	 * parameter = attribute "=" value
	 */
	void SetContentType(MetaEnvs &envs, const server::IRequest &request)
	{
		const HeaderSection::Values &vals = request.Headers()["Content-Type"];
		if (vals.empty()) {
			return;
		}
		// list(#表記)が許可されてないので先頭のみ処理
		envs[cgi::kContentType] = vals.front().GetValue();
	}

	// [RFC3875 4.1.4]
	// The GATEWAY_INTERFACE variable MUST be set to the dialect of CGI being used by the server to
	// communicate with the script.
	// GATEWAY_INTERFACE = "CGI" "/" 1*digit "." 1*digit
	void SetGatewayInterface(MetaEnvs &envs)
	{
		envs[cgi::kGatewayInterface] = "CGI/" + CgiResponse::kCgiVersion;
	}

	// [RFC3875 4.1.5. PATH_INFO] script_nameの後ろ
	// [RFC3875 4.1.6. PATH_TRANSLATED] ROOT/PATH_INFO
	// value is derived in this way irrespective of whether it maps to a valid repository location.
	// [RFC3875 4.1.13. SCRIPT_NAME] The SCRIPT_NAME variable MUST be set to a URI path
	// (not URL-encoded) which could identify the CGI script (rather than the script's output).
	void SetPathEnvs(
		MetaEnvs               &envs,
		const server::IRequest &request,
		const std::string      &root,
		const std::string      &script_name
	)
	{
		envs[cgi::kScriptName] = script_name;
		if (script_name.size() >= request.Path().size()) {
			return;
		}
		envs[cgi::kPathInfo]       = request.Path().substr(script_name.size());
		envs[cgi::kPathTranslated] = utils::JoinPath(root, envs[cgi::kPathInfo]);
	}

	// [RFC3875 4.1.7. QUERY_STRING]
	// The server MUST set this variable;
	//
	// [RFC3875 4.1. Request Meta-Variables]
	// a script cannot distinguish between the two requests
	// http://host/script and http://host/script?
	// as in both cases the QUERY_STRING meta-variable would be NULL.
	void SetQueryString(MetaEnvs &envs, const server::IRequest &request)
	{
		envs[cgi::kQueryString] = request.Query();
	}


	// [RFC3875 4.1.12. REQUEST_METHOD]
	// The REQUEST_METHOD meta-variable MUST be set to the method
	void SetRequestMethod(MetaEnvs &envs, const server::IRequest &request)
	{
		envs[cgi::kRequestMethod] = request.Method();
	}

} // namespace cgi
