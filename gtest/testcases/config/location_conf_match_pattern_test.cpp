#include "gtest.h"

#include "config/server_conf.hpp"
#include "config/server_confs.hpp"
#include "config_exceptions.hpp"

TEST(config, location_conf_match_pattern)
{
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location ^ / {"
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
						conf::LocationConf::MatchPattern::kPrefix
					),
				})
			),
		})
	);
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location = / {"
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
						conf::LocationConf::MatchPattern::kExact
					),
				})
			),
		})
	);
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location $ / {"
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
						conf::LocationConf::MatchPattern::kSuffix
					),
				})
			),
		})
	);

	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / / {"
							  "}"
							  "}"),
		conf::ConfigException
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location . / {"
							  "}"
							  "}"),
		conf::ConfigException
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location ~ / {"
							  "}"
							  "}"),
		conf::ConfigException
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location ^^ / {"
							  "}"
							  "}"),
		conf::ConfigException
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location == / {"
							  "}"
							  "}"),
		conf::ConfigException
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location $$ / {"
							  "}"
							  "}"),
		conf::ConfigException
	);
}
