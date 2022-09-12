#include "parse_authority.hpp"

#include "parse_define.hpp"
#include "parse_ip.hpp"
#include "parse_path_utils.hpp"
#include "parse_uri_utils.hpp"
#include "thin_string.hpp"

namespace ABNF
{
	static const ThinString kUserInfoUniqSet = ":";

	// authority = [ userinfo "@" ] host [ ":" port ]
	bool IsAuthority(const ThinString &str)
	{
		ThinString::ThinStrPair userinfo_hostport_pair = str.DivideBy("@", ThinString::kAlignRight);
		ThinString              userinfo               = userinfo_hostport_pair.first;
		ThinString              hostport               = userinfo_hostport_pair.second;
		if (!IsUserInfo(userinfo)) {
			return false;
		}
		ThinString::ThinStrPair host_port_pair = hostport.DivideBy(":");
		ThinString              host           = host_port_pair.first;
		ThinString              port           = host_port_pair.second;
		return IsHost(host) && IsPort(port);
	}

	// userinfo = *( unreserved / pct-encoded / sub-delims / ":" )
	bool IsUserInfo(const ThinString &str)
	{
		StringAry tokens = TokenizePchar(str);
		return tokens.empty() || IsRegularUriTokenOnly(tokens, kUserInfoUniqSet);
	}

	// host = IP-literal / IPv4address / reg-name
	// TODO テスト追加
	bool IsHost(const ThinString &str)
	{
		return IsIPLiteral(str) || IsIPv4address(str) || IsRegName(str);
	}

	// port = *DIGIT
	bool IsPort(const ThinString &str)
	{
		for (ThinString::const_iterator itr = str.begin(); itr != str.end(); itr++) {
			if (!std::isdigit(*itr)) {
				return false;
			}
		}
		return true;
	}
} // namespace ABNF
