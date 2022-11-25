#include "gtest.h"

#include "config/server_conf.hpp"
#include "config/server_confs.hpp"
#include "config_exceptions.hpp"

TEST(config, server_name)
{
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "root /var/www;"
							  "listen 80;"
							  "server_name localhost;"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::Root("/var/www"),
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"})
			),
		})
	);
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "root /var/www;"
							  "listen 80;"
							  "server_name localhost localhost;"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::Root("/var/www"),
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost", "localhost"})
			),
		})
	);
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "root /var/www;"
							  "listen 80;"
							  "server_name a b c;"
							  "server_name d;"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::Root("/var/www"),
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"a", "b", "c", "d"})
			),
		})
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "root /var/www;"
							  "listen 80;"
							  "server_name ;"
							  "}"),
		conf::ConfigException
	);
}
