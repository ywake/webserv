#include "gtest.h"

#include "config/server_conf.hpp"
#include "config/server_confs.hpp"
#include "config_exceptions.hpp"

TEST(config, error_page)
{
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "error_page 404 /404.html;"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages(std::map<conf::StatusCode, conf::Path>({
					{"404", "/404.html"},
				}))
			),
		})
	);
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "error_page 404 /404.html;"
							  "error_page 500 /500.html;"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages({
					{"404", "/404.html"},
					{"500", "/500.html"},
				})
			),
		})
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "error_page 404;"
							  "}"),
		conf::ConfigException
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "error_page 404 /404.html 404;"
							  "}"),
		conf::ConfigException
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "error_page 40a /404.html;"
							  "}"),
		conf::ConfigException
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "error_page 4040 /404.html;"
							  "}"),
		conf::ConfigException
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "error_page   ;"
							  "}"),
		conf::ConfigException
	);
}
