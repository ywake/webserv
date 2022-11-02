#include "gtest.h"

#include "config/server_conf.hpp"
#include "config/server_confs.hpp"
#include "config_exceptions.hpp"
#include <list>

TEST(config, get_virtual_servers_list)
{
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
			conf::ServerConf(conf::ServerConf::ListenPort({"80"})),
			conf::ServerConf(conf::ServerConf::ListenPort({"80"})),
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
}

TEST(config, listen)
{
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
		conf::ParseConfigFile("server {"
							  "listen 80 80;"
							  "server_name localhost;"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80", "80"}),
				conf::ServerConf::ServerName({"localhost"})
			),
		})
	);
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "listen 8080;"
							  "server_name localhost;"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80", "8080"}),
				conf::ServerConf::ServerName({"localhost"})
			),
		})
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen ;"
							  "server_name localhost;"
							  "}"),
		conf::ConfigException
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80a;"
							  "server_name localhost;"
							  "}"),
		conf::ConfigException
	);
}

TEST(config, server_name)
{
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
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost localhost;"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost", "localhost"})
			),
		})
	);
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name a b c;"
							  "server_name d;"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"a", "b", "c", "d"})
			),
		})
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name ;"
							  "}"),
		conf::ConfigException
	);
}

TEST(config, error_page)
{
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "error_page 404 /404.html;"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages(std::map<conf::StatusCode, conf::Path>({
					{"404", "/404.html"},
				}))
			),
		})
	);
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "error_page 404 /404.html;"
							  "error_page 500 /500.html;"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages({
					{"404", "/404.html"},
					{"500", "/500.html"},
				})
			),
		})
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "error_page 404;"
							  "}"),
		conf::ConfigException
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "error_page 404 /404.html 404;"
							  "}"),
		conf::ConfigException
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "error_page 40a /404.html;"
							  "}"),
		conf::ConfigException
	);
}

TEST(config, client_max_body)
{
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "client_max_body_size 10m;"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages(),
				conf::ServerConf::ClientMaxBodySize(10 * 1024 * 1024)
			),
		})
	);
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "client_max_body_size 10k;"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages(),
				conf::ServerConf::ClientMaxBodySize(10 * 1024)
			),
		})
	);
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "client_max_body_size 10;"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages(),
				conf::ServerConf::ClientMaxBodySize(10)
			),
		})
	);
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "client_max_body_size 10;"
							  "client_max_body_size 100;"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages(),
				conf::ServerConf::ClientMaxBodySize(100)
			),
		})
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "client_max_body_size ;"
							  "}"),
		conf::ConfigException
	);
	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "client_max_body_size 10a;"
							  "}"),
		conf::ConfigException
	);
}

TEST(config, location_conf)
{
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  //   "root /var/www;"
							  "}"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages(),
				conf::ServerConf::ClientMaxBodySize(1UL << 20),
				conf::ServerConf::LocationConfs({
					{"/", conf::LocationConf()},
				})
			),
		})
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
