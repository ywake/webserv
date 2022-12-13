#include "cgi_response.hpp"
#include "i_request.hpp"
#include "webserv_utils.hpp"
#include <sstream>
#include <vector>

namespace cgi
{
	/**
	 * Authorization = credentials
	 * credentials = auth-scheme [ 1*SP ( token68 / #auth-param ) ]
	 * auth-scheme = token
	 * auth-type = "Basic" / "Digest" / token
	 *
	 * auth-schemeから設定
	 * case insensitive
	 */
	void SetAuthType(std::vector<const char *> &envs, const server::IRequest &request)
	{
		HeaderSection::Values vals = request.Headers()["Authorization"];
		if (vals.empty()) {
			return;
		}
		std::string val = vals.front().GetValue();
		if (val.empty()) {
			return;
		}
		std::string auth_type = utils::DivideBy(val, ' ').first;
		if (auth_type.empty()) {
			return;
		}
		std::string auth_type_str = "AUTH_TYPE=" + auth_type;
		envs.push_back(auth_type_str.c_str());
	}

	/**
	 * message-bodyの大きさをオクテット単位で10進数で設定。なければ未設定
	 * 転送符号化や内容符号化を除去した後の長さ
	 */
	void SetContentLength(std::vector<const char *> &envs, const server::IRequest &request)
	{
		const std::vector<char> *body = request.GetBody();
		if (!request.GetMessage().HasMessageBody()) {
			return;
		}
		std::stringstream ss;
		ss << "CONTENT_LENGTH=" << body->size();
		envs.push_back(const_cast<char *>(ss.str().c_str()));
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
	void SetGatewayInterface(std::vector<char *> &args)
	{
		std::string str = "GATEWAY_INTERFACE=CGI/" + CgiResponse::kCgiVersion;
		args.push_back(const_cast<char *>(str.c_str()));
	}
} // namespace cgi
