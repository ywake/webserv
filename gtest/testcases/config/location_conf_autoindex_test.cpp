#include "gtest.h"

#include "config/server_conf.hpp"
#include "config/server_confs.hpp"
#include "config_exceptions.hpp"

TEST(config, location_conf_autoindex)
{
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "autoindex on;"
							  "}"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
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
						conf::LocationConf::Root(),
						conf::LocationConf::IndexFiles(),
						conf::LocationConf::AutoIndex(true)
					),
				})
			),
		})
	);

	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "autoindex off;"
							  "}"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
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
						conf::LocationConf::Root(),
						conf::LocationConf::IndexFiles(),
						conf::LocationConf::AutoIndex(false)
					),
				})
			),
		})
	);

	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "autoindex on;"
							  "autoindex off;"
							  "}"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
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
						conf::LocationConf::Root(),
						conf::LocationConf::IndexFiles(),
						conf::LocationConf::AutoIndex(false)
					),
				})
			),
		})
	);

	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "autoindex on;"
							  "autoindex ;"
							  "}"
							  "}"),
		conf::ConfigException
	);

	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "autoindex aa;"
							  "}"
							  "}"),
		conf::ConfigException
	);
}
