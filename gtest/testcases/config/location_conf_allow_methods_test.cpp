#include "gtest.h"

#include "config/server_conf.hpp"
#include "config/server_confs.hpp"
#include "config_exceptions.hpp"

TEST(config, location_conf_allow_methods)
{
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "root /var/www;"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "allow_methods GET;"
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
						conf::LocationConf::AllowMethods({"GET"})
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
							  "allow_methods GET POST;"
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
						conf::LocationConf::AllowMethods({"GET", "POST"})
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
							  "allow_methods GET POST DELETE;"
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
						conf::LocationConf::AllowMethods({"GET", "POST", "DELETE"})
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
							  "allow_methods POST DELETE;"
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
						conf::LocationConf::AllowMethods({"POST", "DELETE"})
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
							  "allow_methods GET;"
							  "allow_methods POST;"
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
						conf::LocationConf::AllowMethods({"GET", "POST"})
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
							  "allow_methods GET;"
							  "allow_methods ;"
							  "}"
							  "}"),
		conf::ConfigException
	);

	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "root /var/www;"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "allow_methods POST AAAA;"
							  "}"
							  "}"),
		conf::ConfigException
	);
}
