#include "gtest.h"

#include "config/server_conf.hpp"
#include "config/server_confs.hpp"
#include "config_exceptions.hpp"

TEST(config, get_virtual_servers_list)
{

	EXPECT_EQ(conf::ParseConfigFile(""), std::vector<conf::ServerConf>({}));
	EXPECT_EQ(conf::ParseConfigFile("\n"), std::vector<conf::ServerConf>({}));
	EXPECT_EQ(conf::ParseConfigFile("   "), std::vector<conf::ServerConf>({}));

	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}), conf::ServerConf::ServerName({"localhost"})
			),
		})
	);

	EXPECT_EQ(
		conf::ParseConfigFile("\n\nserver {"
							  "\n\nlisten 80;"
							  "\n\nserver_name localhost;"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}), conf::ServerConf::ServerName({"localhost"})
			),
		})
	);

	EXPECT_THROW(
		conf::ParseConfigFile("server\n{"
							  "listen 80;"
							  "server_name localhost;"
							  "}"),
		conf::ConfigException
	);

	EXPECT_THROW(
		conf::ParseConfigFile("server{"
							  "listen 80\n;"
							  "server_name localhost;"
							  "}"),
		conf::ConfigException
	);

	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location /\n {"
							  "}"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages(),
				conf::ServerConf::ClientMaxBodySize(),
				conf::ServerConf::LocationConfs({
					{
						"/\n",
						conf::LocationConf(),
					},
				})
			),
		})
	);

	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location \n/ {"
							  "}"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages(),
				conf::ServerConf::ClientMaxBodySize(),
				conf::ServerConf::LocationConfs({
					{
						"\n/",
						conf::LocationConf(),
					},
				})
			),
		})
	);

	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "}"
							  "server {"
							  "listen 8080;"
							  "server_name localhost;"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}), conf::ServerConf::ServerName({"localhost"})
			),
			conf::ServerConf(
				conf::ServerConf::ListenPort({"8080"}), conf::ServerConf::ServerName({"localhost"})
			),
		})
	);

	EXPECT_EQ(
		conf::ParseConfigFile("server{}server {\n"
							  "}\n"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(),
			conf::ServerConf(),
		})
	);

	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"),
		conf::ConfigException
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "}"
							  "}"),
		conf::ConfigException
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server / {"
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
							  "location / {"
							  "server {"
							  "}"
							  "}"
							  "}"),
		conf::ConfigException
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "locationa / {"
							  "}"
							  "}"),
		conf::ConfigException
	);
	EXPECT_THROW(conf::ParseConfigFile("test;"), conf::ConfigException);
}
