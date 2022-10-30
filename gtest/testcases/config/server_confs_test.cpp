#include "gtest.h"

#include "config/server_conf.hpp"
#include "config/server_confs.hpp"
#include "config_exceptions.hpp"
#include <list>

TEST(config, get_virtual_servers_list)
{
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "}"),
		std::vector<conf::VirtualServerConf>({
			conf::VirtualServerConf({"80"}, {"localhost"}),
		})
	);

	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "}"
							  "server {"
							  "listen 8080;"
							  "server_name localhost;"
							  "}"),
		std::vector<conf::VirtualServerConf>({
			conf::VirtualServerConf({"80"}, {"localhost"}),
			conf::VirtualServerConf({"8080"}, {"localhost"}),
		})
	);

	EXPECT_EQ(
		conf::ParseConfigFile("server{}server {\n"
							  "}\n"),
		std::vector<conf::VirtualServerConf>({
			conf::VirtualServerConf({"80"}),
			conf::VirtualServerConf({"80"}),
		})
	);

	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"),
		conf::ConfigException
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "}"
							  "}"),
		conf::ConfigException
	);
}

TEST(config, listen)
{
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "}"),
		std::vector<conf::VirtualServerConf>({
			conf::VirtualServerConf({"80"}, {"localhost"}),
		})
	);
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80 80;"
							  "server_name localhost;"
							  "}"),
		std::vector<conf::VirtualServerConf>({
			conf::VirtualServerConf({"80", "80"}, {"localhost"}),
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
							  "listen 80a;"
							  "server_name localhost;"
							  "}"),
		conf::ConfigException
	);
}

TEST(config, server_name)
{
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "}"),
		std::vector<conf::VirtualServerConf>({
			conf::VirtualServerConf({"80"}, {"localhost"}),
		})
	);
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost localhost;"
							  "}"),
		std::vector<conf::VirtualServerConf>({
			conf::VirtualServerConf({"80"}, {"localhost", "localhost"}),
		})
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name ;"
							  "}"),
		conf::ConfigException
	);
}

TEST(config, duplicate)
{
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "listen 8080;"
							  "server_name localhost;"
							  "}"),
		std::vector<conf::VirtualServerConf>({
			conf::VirtualServerConf({"80", "8080"}, {"localhost"}),
		})
	);

	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name a b c;"
							  "server_name d;"
							  "}"),
		std::vector<conf::VirtualServerConf>({
			conf::VirtualServerConf({"80"}, {"a", "b", "c", "d"}),
		})
	);
}
