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

	/**
	 * 変数 GATEWAY_INTERFACE は、 鯖がスクリプトと通信するのに使用する CGI
	 * の種類を設定しなければ**なりません**。
	 */
	void SetGatewayInterface(std::vector<const char *> &envs)
	{
		std::string str = "GATEWAY_INTERFACE=CGI/" + CgiResponse::kCgiVersion;
		envs.push_back((str.c_str()));
	}
} // namespace cgi
