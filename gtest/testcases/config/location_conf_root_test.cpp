#include "gtest.h"

#include "config/server_conf.hpp"
#include "config/server_confs.hpp"
#include "config_exceptions.hpp"

TEST(config, location_conf_root)
{
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "root /var/www;"
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
						conf::LocationConf::Root("/var/www")
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
							  "root /var/www;"
							  "root /var/www/html;"
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
						conf::LocationConf::Root("/var/www/html")
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
							  "root /var/www;"
							  "root ;"
							  "}"
							  "}"),
		conf::ConfigException
	);

	// EXPECT_EQ(
	// 	conf::ParseConfigFile("server {"
	// 						  "listen 80;"
	// 						  "server_name localhost;"
	// 						  "location / {"
	// 						  "root /var/www;"
	// 						  "}"
	// 						  "location /api {"
	// 						  "root /var/www/api;"
	// 						  "}"
	// 						  "}"),
	// 	std::vector<conf::ServerConf>({
	// 		conf::ServerConf(
	// 			{"80"},
	// 			{"localhost"},
	// 			{},
	// 			1UL << 20,
	// 			{
	// 				conf::LocationConf("/", "/var/www"),
	// 				conf::LocationConf("/api", "/var/www/api"),
	// 			}
	// 		),
	// 	})
	// );
	// EXPECT_EQ(
	// 	conf::ParseConfigFile("server {"
	// 						  "listen 80;"
	// 						  "server_name localhost;"
	// 						  "location / {"
	// 						  "root /var/www;"
	// 						  "}"
	// 						  "location /api {"
	// 						  "root /var/www/api;"
	// 						  "}"
	// 						  "location /api/v1 {"
	// 						  "root /var/www/api/v1;"
	// 						  "}"
	// 						  "}"),
	// 	std::vector<conf::ServerConf>({
	// 		conf::ServerConf(
	// 			{"80"},
	// 			{"localhost"},
	// 			{},
	// 			1UL << 20,
	// 			{
	// 				conf::LocationConf("/", "/var/www"),
	// 				conf::LocationConf("/api", "/var/www/api"),
	// 				conf::LocationConf("/api/v1", "/var/www/api/v1"),
	// 			}
	// 		),
	// 	})
	// );
}
