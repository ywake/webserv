#include "gtest.h"

#include "config/server_conf.hpp"
#include "config/server_confs.hpp"
#include "config_exceptions.hpp"

TEST(config, location_conf_root)
{
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "root /var/www;"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "root /var/www;"
							  "}"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::Root("/var/www"),
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages(),
				conf::ServerConf::ClientMaxBodySize(),
				conf::ServerConf::LocationConfs({
					conf::LocationConf(
						conf::LocationConf::PathPattern("/"),
						conf::LocationConf::MatchPattern::kPrefix,
						conf::LocationConf::AllowMethods(),
						conf::LocationConf::Redirect(),
						conf::LocationConf::Root("/var/www")
					),
				})
			),
		})
	);

	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "root /var/www;"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "root /var/www;"
							  "root /var/www/html;"
							  "}"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::Root("/var/www"),
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages(),
				conf::ServerConf::ClientMaxBodySize(),
				conf::ServerConf::LocationConfs({
					conf::LocationConf(
						conf::LocationConf::PathPattern("/"),
						conf::LocationConf::MatchPattern::kPrefix,
						conf::LocationConf::AllowMethods(),
						conf::LocationConf::Redirect(),
						conf::LocationConf::Root("/var/www/html")
					),
				})
			),
		})
	);

	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "root /var/www;"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "root /var/www;"
							  "root ;"
							  "}"
							  "}"),
		conf::ConfigException
	);
}
