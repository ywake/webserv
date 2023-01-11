#include "gtest.h"

#include "config/server_conf.hpp"
#include "config/server_confs.hpp"
#include "config_exceptions.hpp"
#include <list>

TEST(config, server_confs)
{
	conf::ServerConfs::ConfsStore store1 = {
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
					conf::LocationConf::Root("/var/www"),
					conf::LocationConf::IndexFiles({"index.html"}),
					conf::LocationConf::AutoIndex(true)
				),
				conf::LocationConf(
					conf::LocationConf::PathPattern(".php"),
					conf::LocationConf::MatchPattern::kPrefix,
					conf::LocationConf::AllowMethods(),
					conf::LocationConf::Redirect(),
					conf::LocationConf::Root(),
					conf::LocationConf::IndexFiles({"index.php"}),
					conf::LocationConf::AutoIndex(),
					conf::LocationConf::CgiPath("/cgi-bin")
				),
			})
		),
		conf::ServerConf(
			conf::ServerConf::Root("/var/www2"),
			conf::ServerConf::ListenPort({"8080"}),
			conf::ServerConf::ServerName({"example.com"}),
			conf::ServerConf::ErrorPages(),
			conf::ServerConf::ClientMaxBodySize(),
			conf::ServerConf::LocationConfs({
				conf::LocationConf(
					conf::LocationConf::PathPattern("/"),
					conf::LocationConf::MatchPattern::kPrefix,
					conf::LocationConf::AllowMethods(),
					conf::LocationConf::Redirect(),
					conf::LocationConf::Root("/var/www2"),
					conf::LocationConf::IndexFiles({"index.html", "index.htm"}),
					conf::LocationConf::AutoIndex(false)
				),
			})
		),
	};

	EXPECT_EQ(
		conf::ServerConfs("server {"
						  "root /var/www;"
						  "listen 80;"
						  "server_name localhost;"
						  "location / {"
						  "root    /var/www;"
						  "index index.html;"
						  "autoindex on;"
						  "}"
						  "location .php {"
						  "cgi_path /cgi-bin;"
						  "index index.php;"
						  "}"
						  "}"
						  "server {"
						  "root /var/www2;"
						  "listen 8080;"
						  "server_name example.com;"
						  "location / {"
						  "root /var/www2;"
						  "index index.html index.htm;"
						  "autoindex off;"
						  "}"
						  "}"),
		conf::ServerConfs(
			store1,
			conf::ServerConfs::ConfsMap({
				{"80",
				 conf::VirtualServerConfs(
					 "localhost",
					 conf::VirtualServerConfs::HostMap{
						 {"localhost", &store1[0]},
					 }
				 )},
				{"8080",
				 conf::VirtualServerConfs(
					 "example.com",
					 conf::VirtualServerConfs::HostMap{
						 {"example.com", &store1[1]},
					 }
				 )},
			})
		)
	);

	EXPECT_THROW(
		conf::ServerConfs("server {"
						  "root /var/www;"
						  "listen 80;"
						  "server_name localhost;"
						  "location / {"
						  "}"
						  "}"
						  "server {"
						  "root /var/www2;"
						  "listen 80;"
						  "server_name localhost;"
						  "location / {"
						  "autoindex off;"
						  "}"
						  "}"),
		conf::ConfigException
	);

	EXPECT_THROW(
		conf::ServerConfs("server {"
						  "root /var/www;"
						  "listen 80;"
						  "location / {"
						  "}"
						  "}"
						  "server {"
						  "root /var/www2;"
						  "listen 80;"
						  "location / {"
						  "autoindex off;"
						  "}"
						  "}"),
		conf::ConfigException
	);

	EXPECT_NO_THROW(conf::ServerConfs("server {"
									  "root /var/www;"
									  "listen 80;"
									  "server_name localhost;"
									  "location / {"
									  "}"
									  "}"
									  "server {"
									  "root /var/www2;"
									  "listen 80;"
									  "server_name localhost2;"
									  "location / {"
									  "autoindex off;"
									  "}"
									  "}"));

	EXPECT_THROW(conf::ServerConfs(""), conf::ConfigException);
}
