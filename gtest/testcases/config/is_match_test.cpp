#include "gtest.h"

#include "config/server_conf.hpp"
#include "config/server_confs.hpp"
#include "config_exceptions.hpp"

TEST(config, location_conf_is_match_prefix)
{
	conf::LocationConf conf(
		conf::LocationConf::PathPattern("/"), conf::LocationConf::MatchPattern::kPrefix
	);

	EXPECT_TRUE(conf.IsMatch("/"));
	EXPECT_TRUE(conf.IsMatch("/api"));
	EXPECT_TRUE(conf.IsMatch("/test/aaa"));

	EXPECT_FALSE(conf.IsMatch("aaa"));
	EXPECT_FALSE(conf.IsMatch("aaa/"));
	EXPECT_FALSE(conf.IsMatch("aaa/test"));
}

TEST(config, location_conf_is_match_exact)
{
	conf::LocationConf conf(
		conf::LocationConf::PathPattern("/"), conf::LocationConf::MatchPattern::kExact
	);

	EXPECT_TRUE(conf.IsMatch("/"));

	EXPECT_FALSE(conf.IsMatch("/api"));
	EXPECT_FALSE(conf.IsMatch("/test/aaa"));
	EXPECT_FALSE(conf.IsMatch("aaa"));
	EXPECT_FALSE(conf.IsMatch("aaa/"));
	EXPECT_FALSE(conf.IsMatch("aaa/test"));
}

TEST(config, location_conf_is_match_suffix)
{
	conf::LocationConf conf(
		conf::LocationConf::PathPattern("/"), conf::LocationConf::MatchPattern::kSuffix
	);

	EXPECT_TRUE(conf.IsMatch("/"));
	EXPECT_TRUE(conf.IsMatch("aaa/"));

	EXPECT_FALSE(conf.IsMatch("/api"));
	EXPECT_FALSE(conf.IsMatch("/test/aaa"));
	EXPECT_FALSE(conf.IsMatch("aaa"));
	EXPECT_FALSE(conf.IsMatch("aaa/test"));

	conf::LocationConf conf1(
		conf::LocationConf::PathPattern(".php"), conf::LocationConf::MatchPattern::kSuffix
	);
	EXPECT_TRUE(conf1.IsMatch("/api/test.php"));

	EXPECT_FALSE(conf1.IsMatch("/api/test.php/"));
	EXPECT_FALSE(conf1.IsMatch("/api/test.php/aaa"));
}
