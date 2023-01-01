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
	void SetContentLength(std::vector<const char *> &envs, const server::IRequest &request)
	{
		const std::vector<char> *body = request.GetBody();
		if (!request.HasMessageBody()) {
			return;
		}
		std::stringstream ss;
		ss << "CONTENT_LENGTH=" << body->size();
		envs.push_back(ss.str().c_str());
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
	void SetContentType(std::vector<const char *> &envs, const server::IRequest &request)
	{
		HeaderSection::Values vals = request.Headers()["Content-Type"];
		if (vals.empty()) {
			return;
		}
		// list(#表記)が許可されてないので先頭のみ処理
		std::string val = vals.front().GetValue();
		if (val.empty()) {
			return;
		}
		std::string content_type_str = "CONTENT_TYPE=" + val;
		envs.push_back(content_type_str.c_str());
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
