#include "gtest.h"

#include "config/server_conf.hpp"
#include "config/server_confs.hpp"
#include "config_exceptions.hpp"

TEST(config, location_conf_cgi_path)
{
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "cgi_path /cgi-bin;"
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
						conf::LocationConf::AutoIndex(),
						conf::LocationConf::CgiPath("/cgi-bin")
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
							  "cgi_path /cgi-bin;"
							  "cgi_path /cgi-bin2;"
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
						conf::LocationConf::AutoIndex(),
						conf::LocationConf::CgiPath("/cgi-bin2")
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
							  "cgi_path ;"
							  "}"
							  "}"),
		conf::ConfigException
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "cgi_path /cgi-bin /cgi-bin;"
							  "}"
							  "}"),
		conf::ConfigException
	);
}
