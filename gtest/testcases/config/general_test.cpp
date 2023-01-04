#include "gtest.h"

#include "config/server_conf.hpp"
#include "config/server_confs.hpp"
#include "config_exceptions.hpp"

TEST(config, get_virtual_servers_list)
{

	EXPECT_EQ(conf::ParseConfigFile(""), std::vector<conf::ServerConf>({}));
	EXPECT_THROW(conf::ParseConfigFile("\n"), conf::ConfigException);
	EXPECT_THROW(conf::ParseConfigFile("   "), conf::ConfigException);

	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "root /var/www;"
							  "listen 80;"
							  "server_name localhost;"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::Root("/var/www"),
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"})
			),
		})
	);

	EXPECT_EQ(
		conf::ParseConfigFile("\n\nserver {"
							  "\n\nroot /var/www;"
							  "\n\nlisten 80;"
							  "\n\nserver_name localhost;"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::Root("/var/www"),
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"})
			),
		})
	);

	EXPECT_THROW(
		conf::ParseConfigFile("server\n{"
							  "root /var/www;"
							  "listen 80;"
							  "server_name localhost;"
							  "}"),
		conf::ConfigException
	);

	EXPECT_THROW(
		conf::ParseConfigFile("server{"
							  "root /var/www;"
							  "listen 80\n;"
							  "server_name localhost;"
							  "}"),
		conf::ConfigException
	);

	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "root /var/www;"
							  "listen 80;"
							  "server_name localhost;"
							  "location /\n {"
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
					conf::LocationConf(conf::LocationConf::PathPattern("/\n")),
				})
			),
		})
	);

	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "root /var/www;"
							  "listen 80;"
							  "server_name localhost;"
							  "location \n/ {"
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
					conf::LocationConf(conf::LocationConf::PathPattern("\n/")),
				})
			),
		})
	);

	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "root /var/www;"
							  "listen 80;"
							  "server_name localhost;"
							  "}"
							  "server {"
							  "root /var/www;"
							  "listen 8080;"
							  "server_name localhost;"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::Root("/var/www"),
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"})
			),
			conf::ServerConf(
				conf::ServerConf::Root("/var/www"),
				conf::ServerConf::ListenPort({"8080"}),
				conf::ServerConf::ServerName({"localhost"})
			),
		})
	);

	EXPECT_THROW(
		conf::ParseConfigFile("server{root /var/www;}server {\n"
							  "root /var/www;"
							  "}\n"),
		conf::ConfigException
	);

	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "root /var/www;"
							  "listen 80;"
							  "server_name localhost;"),
		conf::ConfigException
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "root /var/www;"
							  "listen 80;"
							  "server_name localhost;"
							  "}"
							  "}"),
		conf::ConfigException
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server / {"
							  "root /var/www;"
							  "aaaa"
							  "}"),
		conf::ConfigException
	);
	EXPECT_THROW(
		conf::ParseConfigFile("location / {"
							  "}"),
		conf::ConfigException
	);
	EXPECT_THROW(
		conf::ParseConfigFile("location / {"
							  "location / {"
							  "}"
							  "}"),
		conf::ConfigException
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "root /var/www;"
							  "location / {"
							  "server {"
							  "root /var/www;"
							  "}"
							  "}"
							  "}"),
		conf::ConfigException
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "root /var/www;"
							  "locationa / {"
							  "}"
							  "}"),
		conf::ConfigException
	);
	EXPECT_THROW(conf::ParseConfigFile("test;"), conf::ConfigException);
}
