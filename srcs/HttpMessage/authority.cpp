#include "authority.hpp"

#include "error.hpp"
#include "parse_authority.hpp"

Authority::Authority(ThinString authority)
{
	Tokens tokens = TokenizeAuthority(authority);
	if (!ABNF::IsUserInfo(tokens.userinfo) || !ABNF::IsHost(tokens.host) ||
		!ABNF::IsPort(tokens.port)) {
		throw Error("400");
	}
	userinfo_ = tokens.userinfo.ToString();
	host_ = tokens.host.ToString();
	port_ = tokens.port.ToString();
}

// TODO ここもうちょい上手く書けるかも
Authority::Tokens Authority::TokenizeAuthority(ThinString authority)
{
	Tokens tokens;
	ThinString::ThinStrPair userinfo_hostport = authority.DivideBy("@", ThinString::kAlignRight);
	ThinString hostport = userinfo_hostport.second;
	ThinString::ThinStrPair host_port = hostport.DivideBy(":");
	tokens.userinfo = userinfo_hostport.first;
	tokens.host = host_port.first;
	tokens.port = host_port.second;
	return tokens;
}
