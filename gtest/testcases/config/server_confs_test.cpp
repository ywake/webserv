#include "gtest.h"

#include "config/server_conf.hpp"
#include "config/server_confs.hpp"
#include "config_exceptions.hpp"
#include <list>

TEST(ServerConfs, get_virtual_servers_list)
{
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "}"),
		std::vector<conf::VirtualServerConf>({
			conf::VirtualServerConf("80", "localhost"),
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
			conf::VirtualServerConf("80", "localhost"),
			conf::VirtualServerConf("8080", "localhost"),
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

	// EXPECT_THROW(
	// 	conf::ParseConfigFile(" server {\n"
	// 						  "}\n"),
	// 	conf::ConfigException
	// );
	// EXPECT_THROW(
	// 	conf::ParseConfigFile("server{\n"
	// 						  "}\n"),
	// 	conf::ConfigException
	// );
	// EXPECT_THROW(
	// 	conf::ParseConfigFile("server\n"
	// 						  "{\n"
	// 						  "}\n"),
	// 	conf::ConfigException
	// );
	// EXPECT_THROW(
	// 	conf::ParseConfigFile("server {\n"
	// 						  "} \n"),
	// 	conf::ConfigException
	// );
	// EXPECT_THROW(
	// 	conf::ParseConfigFile("\n"
	// 						  "server {\n"
	// 						  "}\n"),
	// 	conf::ConfigException
	// );
	// EXPECT_THROW(
	// 	conf::ParseConfigFile("server {\n"
	// 						  "}\n"
	// 						  "\n"
	// 						  "server {\n"
	// 						  "}\n"),
	// 	conf::ConfigException
	// );
}
