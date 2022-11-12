#include "gtest.h"

#include "config/server_conf.hpp"
#include "config/server_confs.hpp"
#include "config_exceptions.hpp"

TEST(config, location_conf_redirect)
{
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "redirect 301 example.com;"
							  "}"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages(),
				conf::ServerConf::ClientMaxBodySize(),
				conf::ServerConf::LocationConfs({
					{
						"/",
						conf::LocationConf(
							conf::LocationConf::AllowMethods(),
							conf::LocationConf::Redirect({"301", "example.com"})
						),
					},
				})
			),
		})
	);

	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "redirect 301 example.com;"
							  "redirect 302 example.com;"
							  "}"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages(),
				conf::ServerConf::ClientMaxBodySize(),
				conf::ServerConf::LocationConfs({
					{
						"/",
						conf::LocationConf(
							conf::LocationConf::AllowMethods(),
							conf::LocationConf::Redirect({"302", "example.com"})
						),
					},
				})
			),
		})
	);

	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "redirect ;"
							  "}"
							  "}"),
		conf::ConfigException
	);

	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "redirect 301;"
							  "}"
							  "}"),
		conf::ConfigException
	);

	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "redirect 304 example.com;"
							  "}"
							  "}"),
		conf::ConfigException
	);

	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "redirect 200 example.com;"
							  "}"
							  "}"),
		conf::ConfigException
	);

	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "redirect AAAAA example.com;"
							  "}"
							  "}"),
		conf::ConfigException
	);
}
