#include "gtest.h"

#include "cgi/get_resource_pair.hpp"

TEST(cgi_response_helper_test, is_end_with_slash)
{
	EXPECT_TRUE(cgi::IsEndWithSlash("/"));
	EXPECT_TRUE(cgi::IsEndWithSlash("/a/"));
	EXPECT_FALSE(cgi::IsEndWithSlash("/a"));
	EXPECT_FALSE(cgi::IsEndWithSlash(""));
}

TEST(cgi_response_helper_test, create_path_pair_candidates)
{
	EXPECT_EQ(
		cgi::CreatePathPairCandidates("/a/b/c/d"),
		std::vector<cgi::PathPair>({
			cgi::PathPair("/a", "/b/c/d"),
			cgi::PathPair("/a/b", "/c/d"),
			cgi::PathPair("/a/b/c", "/d"),
			cgi::PathPair("/a/b/c/d", ""),
		})
	);
	EXPECT_EQ(
		cgi::CreatePathPairCandidates("a/b/c/d"),
		std::vector<cgi::PathPair>({
			cgi::PathPair("a", "/b/c/d"),
			cgi::PathPair("a/b", "/c/d"),
			cgi::PathPair("a/b/c", "/d"),
			cgi::PathPair("a/b/c/d", ""),
		})
	);
	EXPECT_EQ(
		cgi::CreatePathPairCandidates("a/b/c/d/"),
		std::vector<cgi::PathPair>({
			cgi::PathPair("a", "/b/c/d/"),
			cgi::PathPair("a/b", "/c/d/"),
			cgi::PathPair("a/b/c", "/d/"),
			cgi::PathPair("a/b/c/d", ""),
		})
	);
	EXPECT_EQ(cgi::CreatePathPairCandidates(""), std::vector<cgi::PathPair>({}));
	EXPECT_EQ(cgi::CreatePathPairCandidates("/"), std::vector<cgi::PathPair>({}));
}
