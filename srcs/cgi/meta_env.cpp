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
	void SetAuthType(std::vector<char *> &args, const server::IRequest &request)
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
		args.push_back(const_cast<char *>(auth_type_str.c_str()));
	}

	/**
	 * message-bodyの大きさをオクテット単位で10進数で設定。なければ未設定
	 * 転送符号化や内容符号化を除去した後の長さ
	 */
	void SetContentLength(std::vector<char *> &args, const server::IRequest &request)
	{
		const std::vector<char> *body = request.GetBody();
		if (body == NULL || body->empty()) {
			return;
		}
		std::stringstream ss;
		ss << "CONTENT_LENGTH=" << body->size();
		args.push_back(const_cast<char *>(ss.str().c_str()));
	}
} // namespace cgi
