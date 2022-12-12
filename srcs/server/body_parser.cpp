#include "body_parser.hpp"

namespace server
{
	// TODO あとでconfとかに動かしたい
	const conf::ServerConf &
	GetServerConf(const conf::VirtualServerConfs &config, const HeaderSection::Values &hosts)
	{
		if (hosts.empty()) {
			return config.GetDefaultServerConf();
		} else {
			const std::string &host = hosts.front().GetValue();
			return config[host];
		}
	}

} // namespace server
