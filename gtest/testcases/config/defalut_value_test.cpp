#include "gtest.h"

#include "config/location_conf.hpp"
#include "config/server_conf.hpp"

TEST(config, default_server_conf)
{
	conf::ServerConf server_conf;
	EXPECT_EQ(server_conf.GetListenPort(), conf::ServerConf::ListenPort({"80"}));
	EXPECT_EQ(server_conf.GetServerName(), conf::ServerConf::ServerName({""}));
	EXPECT_EQ(server_conf.GetErrorPages(), conf::ServerConf::ErrorPages());
	EXPECT_EQ(server_conf.GetClientMaxBodySize(), 1 << 20);
	EXPECT_EQ(server_conf.GetLocationConfs(), conf::ServerConf::LocationConfs());
	EXPECT_EQ(server_conf.FindMatchingLocationConf("/"), conf::LocationConf());
}

// location
TEST(config, default_location_conf)
{
	conf::LocationConf location_conf(conf::LocationConf::PathPattern("/"));
	EXPECT_EQ(location_conf.GetPathPattern(), conf::Path("/"));
	EXPECT_EQ(location_conf.GetMatchPattern(), conf::LocationConf::MatchPattern::kPrefix);
	EXPECT_EQ(location_conf.GetAllowMethods(), conf::LocationConf::AllowMethods({"GET"}));
	EXPECT_EQ(location_conf.GetRedirect(), conf::LocationConf::Redirect());
	EXPECT_EQ(location_conf.GetRawRoot(), conf::LocationConf::Root());
	EXPECT_EQ(location_conf.GetIndexFiles(), conf::LocationConf::IndexFiles({"index.html"}));
	EXPECT_EQ(location_conf.GetAutoindex(), conf::LocationConf::AutoIndex());
	EXPECT_EQ(location_conf.GetCgiPath(), conf::LocationConf::CgiPath());
}
