#include "gtest.h"

#include "config/server_conf.hpp"
#include "config/server_confs.hpp"
#include "config_exceptions.hpp"

TEST(config, location_conf_index_files)
{
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "root /var/www;"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "index_files index.html;"
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
						conf::LocationConf::Root(),
						conf::LocationConf::IndexFiles({"index.html"})
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
							  "index_files index.html index.htm;"
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
						conf::LocationConf::Root(),
						conf::LocationConf::IndexFiles({"index.html", "index.htm"})
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
							  "index_files a b;"
							  "index_files c d;"
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
						conf::LocationConf::Root(),
						conf::LocationConf::IndexFiles({"a", "b", "c", "d"})
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
							  "index_files index.html;"
							  "index_files ;"
							  "}"
							  "}"),
		conf::ConfigException
	);
}
