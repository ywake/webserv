#include "gtest.h"

#include "config/server_conf.hpp"
#include "config/server_confs.hpp"
#include "config_exceptions.hpp"

TEST(config, listen)
{
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}), conf::ServerConf::ServerName({"localhost"})
			),
		})
	);
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80 80;"
							  "server_name localhost;"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80", "80"}),
				conf::ServerConf::ServerName({"localhost"})
			),
		})
	);
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "listen 8080;"
							  "server_name localhost;"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80", "8080"}),
				conf::ServerConf::ServerName({"localhost"})
			),
		})
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen ;"
							  "server_name localhost;"
							  "}"),
		conf::ConfigException
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen  ;"
							  "server_name localhost;"
							  "}"),
		conf::ConfigException
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80a;"
							  "server_name localhost;"
							  "}"),
		conf::ConfigException
	);
}
