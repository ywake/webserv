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

TEST(config, location_conf_allow_methods)
{
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "allow_methods GET;"
							  "}"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages(),
				conf::ServerConf::ClientMaxBodySize(1UL << 20),
				conf::ServerConf::LocationConfs(std::map<conf::Path, conf::LocationConf>{
					{
						"/",
						conf::LocationConf(conf::LocationConf::AllowMethods({"GET"})),
					},
				})
			),
		})
	);

	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "allow_methods GET POST;"
							  "}"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages(),
				conf::ServerConf::ClientMaxBodySize(1UL << 20),
				conf::ServerConf::LocationConfs(std::map<conf::Path, conf::LocationConf>{
					{
						"/",
						conf::LocationConf(conf::LocationConf::AllowMethods({"GET", "POST"})),
					},
				})
			),
		})
	);

	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "allow_methods GET POST DELETE;"
							  "}"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages(),
				conf::ServerConf::ClientMaxBodySize(1UL << 20),
				conf::ServerConf::LocationConfs(std::map<conf::Path, conf::LocationConf>{
					{
						"/",
						conf::LocationConf(
							conf::LocationConf::AllowMethods({"GET", "POST", "DELETE"})
						),
					},
				})
			),
		})
	);

	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "allow_methods POST DELETE;"
							  "}"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages(),
				conf::ServerConf::ClientMaxBodySize(1UL << 20),
				conf::ServerConf::LocationConfs(std::map<conf::Path, conf::LocationConf>{
					{
						"/",
						conf::LocationConf(conf::LocationConf::AllowMethods({"POST", "DELETE"})),
					},
				})
			),
		})
	);

	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "allow_methods POST AAAA;"
							  "}"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages(),
				conf::ServerConf::ClientMaxBodySize(1UL << 20),
				conf::ServerConf::LocationConfs(std::map<conf::Path, conf::LocationConf>{
					{
						"/",
						conf::LocationConf(conf::LocationConf::AllowMethods({"POST", "AAAA"})),
					},
				})
			),
		})
	);

	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "allow_methods GET;"
							  "allow_methods POST;"
							  "}"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages(),
				conf::ServerConf::ClientMaxBodySize(1UL << 20),
				conf::ServerConf::LocationConfs(std::map<conf::Path, conf::LocationConf>{
					{
						"/",
						conf::LocationConf(conf::LocationConf::AllowMethods({"GET", "POST"})),
					},
				})
			),
		})
	);

	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "allow_methods GET;"
							  "allow_methods ;"
							  "}"
							  "}"),
		conf::ConfigException
	);
}

TEST(config, location_conf_redirect)
{
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "redirect 301 example.com;"
							  "}"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages(),
				conf::ServerConf::ClientMaxBodySize(1UL << 20),
				conf::ServerConf::LocationConfs(std::map<conf::Path, conf::LocationConf>{
					{
						"/",
						conf::LocationConf(
							conf::LocationConf::AllowMethods(),
							conf::LocationConf::Redirect({"301", "example.com"})
						),
					},
				})
			),
		})
	);

	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "redirect 301 example.com;"
							  "redirect 302 example.com;"
							  "}"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages(),
				conf::ServerConf::ClientMaxBodySize(1UL << 20),
				conf::ServerConf::LocationConfs(std::map<conf::Path, conf::LocationConf>{
					{
						"/",
						conf::LocationConf(
							conf::LocationConf::AllowMethods(),
							conf::LocationConf::Redirect({"302", "example.com"})
						),
					},
				})
			),
		})
	);

	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "redirect ;"
							  "}"
							  "}"),
		conf::ConfigException
	);

	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "redirect 301;"
							  "}"
							  "}"),
		conf::ConfigException
	);
}

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
				conf::ServerConf::ClientMaxBodySize(1UL << 20),
				conf::ServerConf::LocationConfs(std::map<conf::Path, conf::LocationConf>{
					{
						"/",
						conf::LocationConf(
							conf::LocationConf::AllowMethods(),
							conf::LocationConf::Redirect(),
							conf::LocationConf::Root("/var/www")
						),
					},
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
				conf::ServerConf::ClientMaxBodySize(1UL << 20),
				conf::ServerConf::LocationConfs(std::map<conf::Path, conf::LocationConf>{
					{
						"/",
						conf::LocationConf(
							conf::LocationConf::AllowMethods(),
							conf::LocationConf::Redirect(),
							conf::LocationConf::Root("/var/www/html")
						),
					},
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

TEST(config, location_conf_index_files)
{
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "index_files index.html;"
							  "}"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages(),
				conf::ServerConf::ClientMaxBodySize(1UL << 20),
				conf::ServerConf::LocationConfs(std::map<conf::Path, conf::LocationConf>{
					{
						"/",
						conf::LocationConf(
							conf::LocationConf::AllowMethods(),
							conf::LocationConf::Redirect(),
							conf::LocationConf::Root(),
							conf::LocationConf::IndexFiles({"index.html"})
						),
					},
				})
			),
		})
	);

	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "index_files index.html index.htm;"
							  "}"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages(),
				conf::ServerConf::ClientMaxBodySize(1UL << 20),
				conf::ServerConf::LocationConfs(std::map<conf::Path, conf::LocationConf>{
					{
						"/",
						conf::LocationConf(
							conf::LocationConf::AllowMethods(),
							conf::LocationConf::Redirect(),
							conf::LocationConf::Root(),
							conf::LocationConf::IndexFiles({"index.html", "index.htm"})
						),
					},
				})
			),
		})
	);

	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "index_files a b;"
							  "index_files c d;"
							  "}"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages(),
				conf::ServerConf::ClientMaxBodySize(1UL << 20),
				conf::ServerConf::LocationConfs(std::map<conf::Path, conf::LocationConf>{
					{
						"/",
						conf::LocationConf(
							conf::LocationConf::AllowMethods(),
							conf::LocationConf::Redirect(),
							conf::LocationConf::Root(),
							conf::LocationConf::IndexFiles({"a", "b", "c", "d"})
						),
					},
				})
			),
		})
	);

	EXPECT_THROW(
		conf::ParseConfigFile("server {"
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

TEST(config, location_conf_autoindex)
{
	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "autoindex on;"
							  "}"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages(),
				conf::ServerConf::ClientMaxBodySize(1UL << 20),
				conf::ServerConf::LocationConfs(std::map<conf::Path, conf::LocationConf>{
					{
						"/",
						conf::LocationConf(
							conf::LocationConf::AllowMethods(),
							conf::LocationConf::Redirect(),
							conf::LocationConf::Root(),
							conf::LocationConf::IndexFiles(),
							conf::LocationConf::AutoIndex(true)
						),
					},
				})
			),
		})
	);

	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "autoindex off;"
							  "}"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages(),
				conf::ServerConf::ClientMaxBodySize(1UL << 20),
				conf::ServerConf::LocationConfs(std::map<conf::Path, conf::LocationConf>{
					{
						"/",
						conf::LocationConf(
							conf::LocationConf::AllowMethods(),
							conf::LocationConf::Redirect(),
							conf::LocationConf::Root(),
							conf::LocationConf::IndexFiles(),
							conf::LocationConf::AutoIndex(false)
						),
					},
				})
			),
		})
	);

	EXPECT_EQ(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "autoindex on;"
							  "autoindex off;"
							  "}"
							  "}"),
		std::vector<conf::ServerConf>({
			conf::ServerConf(
				conf::ServerConf::ListenPort({"80"}),
				conf::ServerConf::ServerName({"localhost"}),
				conf::ServerConf::ErrorPages(),
				conf::ServerConf::ClientMaxBodySize(1UL << 20),
				conf::ServerConf::LocationConfs(std::map<conf::Path, conf::LocationConf>{
					{
						"/",
						conf::LocationConf(
							conf::LocationConf::AllowMethods(),
							conf::LocationConf::Redirect(),
							conf::LocationConf::Root(),
							conf::LocationConf::IndexFiles(),
							conf::LocationConf::AutoIndex(false)
						),
					},
				})
			),
		})
	);

	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "autoindex on;"
							  "autoindex ;"
							  "}"
							  "}"),
		conf::ConfigException
	);

	EXPECT_THROW(
		conf::ParseConfigFile("server {"
							  "listen 80;"
							  "server_name localhost;"
							  "location / {"
							  "autoindex aa;"
							  "}"
							  "}"),
		conf::ConfigException
	);
}

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
				conf::ServerConf::ClientMaxBodySize(1UL << 20),
				conf::ServerConf::LocationConfs(std::map<conf::Path, conf::LocationConf>{
					{
						"/",
						conf::LocationConf(
							conf::LocationConf::AllowMethods(),
							conf::LocationConf::Redirect(),
							conf::LocationConf::Root(),
							conf::LocationConf::IndexFiles(),
							conf::LocationConf::AutoIndex(),
							conf::LocationConf::CgiPath("/cgi-bin")
						),
					},
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
				conf::ServerConf::ClientMaxBodySize(1UL << 20),
				conf::ServerConf::LocationConfs(std::map<conf::Path, conf::LocationConf>{
					{
						"/",
						conf::LocationConf(
							conf::LocationConf::AllowMethods(),
							conf::LocationConf::Redirect(),
							conf::LocationConf::Root(),
							conf::LocationConf::IndexFiles(),
							conf::LocationConf::AutoIndex(),
							conf::LocationConf::CgiPath("/cgi-bin2")
						),
					},
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
