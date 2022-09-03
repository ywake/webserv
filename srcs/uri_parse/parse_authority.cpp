#include "parse_authority.hpp"

#include "ThinString.hpp"
#include "parse_define.hpp"
#include "parse_ip.hpp"
#include "parse_path_utils.hpp"
#include "parse_uri_utils.hpp"

namespace ABNF
{
	static const char *kUserInfoUniqSet = ":";

	// authority = [ userinfo "@" ] host [ ":" port ]
	bool IsAuthority(const ThinString &str)
	{
		ThinString::ThinStrPair userinfo_hostport_pair = str.DivideBy("@");
		ThinString userinfo = userinfo_hostport_pair.first;
		ThinString hostport = userinfo_hostport_pair.second;
		if (!IsUserInfo(userinfo)) {
			return false;
		}
		ThinString::ThinStrPair host_port_pair = hostport.DivideBy(":");
		ThinString host = host_port_pair.first;
		ThinString port = host_port_pair.second;
		return IsHost(host) && IsPort(port);
	}

	// userinfo = *( unreserved / pct-encoded / sub-delims / ":" )
	bool IsUserInfo(const ThinString &str)
	{
		StringAry tokens = TokenizePchar(str);
		return IsRegularURITokenAll(tokens, kUserInfoUniqSet);
	}

	// host = IP-literal / IPv4address / reg-name
	// http URI scheme : A sender MUST NOT generate an "http" URI with an empty host identifier.
	// A recipient that processes such a URI reference MUST reject it as invalid.
	// TODO IsHTTPHostみたいの作ってwrapした方がいいかも
	// TODO テスト追加
	bool IsHost(const ThinString &str)
	{
		if (str.empty()) {
			return false;
		}
		return IsIPLiteral(str) || IsIPv4address(str) || IsRegName(str);
	}

	// port = *DIGIT
	bool IsPort(const ThinString &str)
	{
		(void)str;
		return false;
	}
} // namespace ABNF
