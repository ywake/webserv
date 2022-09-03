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
		for (StringAry::const_iterator it = tokens.begin(); it != tokens.end(); it++) {
			if (!IsRegularURIToken(*it, kUserInfoUniqSet)) {
				return false;
			}
		}
		return true;
	}

	// host = IP-literal / IPv4address / reg-name
	bool IsHost(const ThinString &str)
	{
		return IsIPLiteral(str) || IsIPv4address(str) || IsRegName(str);
	}

	// port = *DIGIT
	bool IsPort(const ThinString &str)
	{
		(void)str;
		return false;
	}
} // namespace ABNF
