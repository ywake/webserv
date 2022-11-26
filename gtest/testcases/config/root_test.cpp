#include "gtest.h"

#include "config/server_conf.hpp"
#include "config/server_confs.hpp"
#include "config_exceptions.hpp"

TEST(config, root)
{
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "root /var/www;"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(conf::ServerConf::Root("/var/www")),
		})
	);

	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "root;"
							  "}"),
		conf::ConfigException
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "root /var/www /var/www;"
							  "}"),
		conf::ConfigException
	);
}
