#include "gtest.h"

#include "config/server_conf.hpp"
#include "config/server_confs.hpp"
#include "config_exceptions.hpp"
#include <list>

TEST(config, server_confs)
{
	conf::ServerConfs::ConfsStore store1 = {
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
						conf::LocationConf::Redirect(),
						conf::LocationConf::Root("/var/www"),
						conf::LocationConf::IndexFiles({"index.html"}),
						conf::LocationConf::AutoIndex(true)
					),
				},
				{
					".php",
					conf::LocationConf(
						conf::LocationConf::AllowMethods(),
						conf::LocationConf::Redirect(),
						conf::LocationConf::Root(),
						conf::LocationConf::IndexFiles({"index.php"}),
						conf::LocationConf::AutoIndex(),
						conf::LocationConf::CgiPath("/cgi-bin")
					),
				},
			})
		),
		conf::ServerConf(
			conf::ServerConf::ListenPort({"8080"}),
			conf::ServerConf::ServerName({"example.com"}),
			conf::ServerConf::ErrorPages(),
			conf::ServerConf::ClientMaxBodySize(),
			conf::ServerConf::LocationConfs({
				{
					"/",
					conf::LocationConf(
						conf::LocationConf::AllowMethods(),
						conf::LocationConf::Redirect(),
						conf::LocationConf::Root("/var/www2"),
						conf::LocationConf::IndexFiles({"index.html", "index.htm"}),
						conf::LocationConf::AutoIndex(false)
					),
				},
			})
		),
	};

	EXPECT_EQ(
		conf::ServerConfs("server {"
						  "listen 80;"
						  "server_name localhost;"
						  "location / {"
						  "root    /var/www;"
						  "index_files index.html;"
						  "autoindex on;"
						  "}"
						  "location .php {"
						  "cgi_path /cgi-bin;"
						  "index_files index.php;"
						  "}"
						  "}"
						  "server {"
						  "listen 8080;"
						  "server_name example.com;"
						  "location / {"
						  "root /var/www2;"
						  "index_files index.html index.htm;"
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

	EXPECT_THROW(conf::ServerConfs(""), conf::ConfigException);
}
