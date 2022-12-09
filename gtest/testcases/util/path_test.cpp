#include "gtest.h"
#include "webserv_utils.hpp"

using namespace utils;

TEST(normalize_path, ok)
{
	EXPECT_EQ(NormalizePath(""), "/");
	EXPECT_EQ(NormalizePath("/"), "/");
	EXPECT_EQ(NormalizePath("/a/"), "/a/");
	EXPECT_EQ(NormalizePath("a"), "/a");
	EXPECT_EQ(NormalizePath("/a"), "/a");
	EXPECT_EQ(NormalizePath("a/.."), "/");
	EXPECT_EQ(NormalizePath("a/../"), "/");
	EXPECT_EQ(NormalizePath("/a/.."), "/");
	EXPECT_EQ(NormalizePath("/a/../"), "/");
	EXPECT_EQ(NormalizePath("a/."), "/a/");
	EXPECT_EQ(NormalizePath("a/./"), "/a/");
	EXPECT_EQ(NormalizePath("/a/."), "/a/");
	EXPECT_EQ(NormalizePath("/a/./"), "/a/");
	EXPECT_EQ(NormalizePath("./"), "/");
	EXPECT_EQ(NormalizePath("a//"), "/a/");
	EXPECT_EQ(NormalizePath("..a"), "/..a");
	EXPECT_EQ(NormalizePath("..a/"), "/..a/");
	EXPECT_EQ(NormalizePath("a.."), "/a..");
	EXPECT_EQ(NormalizePath("/a.."), "/a..");
	EXPECT_EQ(NormalizePath("a../"), "/a../");
	EXPECT_EQ(NormalizePath(".a"), "/.a");
	EXPECT_EQ(NormalizePath(".a/"), "/.a/");
	EXPECT_EQ(NormalizePath("a."), "/a.");
	EXPECT_EQ(NormalizePath("/a."), "/a.");
	EXPECT_EQ(NormalizePath("a./"), "/a./");
	EXPECT_EQ(NormalizePath("../a/./../.././b/./../"), "/");
	EXPECT_EQ(NormalizePath("../../a/../b/c/.././d/../"), "/b/");
	EXPECT_EQ(NormalizePath("/a/b/c/./../../g"), "/a/g");
}

TEST(join_path, base_case)
{
	EXPECT_EQ(JoinPath("", ""), "");
	EXPECT_EQ(JoinPath("/", ""), "/");
	EXPECT_EQ(JoinPath("", "/"), "/");
	EXPECT_EQ(JoinPath("/", "/"), "/");

	EXPECT_EQ(JoinPath("a", ""), "a");
	EXPECT_EQ(JoinPath("", "b"), "b");
	EXPECT_EQ(JoinPath("a", "b"), "a/b");
	EXPECT_EQ(JoinPath("a/", "/b"), "a/b");
	EXPECT_EQ(JoinPath("a/", "b"), "a/b");
	EXPECT_EQ(JoinPath("a", "/b"), "a/b");

	EXPECT_EQ(JoinPath("/a", ""), "/a");
	EXPECT_EQ(JoinPath("/", "b"), "/b");
	EXPECT_EQ(JoinPath("/a", "b"), "/a/b");
	EXPECT_EQ(JoinPath("/a/", "/b"), "/a/b");
	EXPECT_EQ(JoinPath("/a/", "b"), "/a/b");
	EXPECT_EQ(JoinPath("/a", "/b"), "/a/b");

	EXPECT_EQ(JoinPath("a", "/"), "a/");
	EXPECT_EQ(JoinPath("", "/b"), "/b");
	EXPECT_EQ(JoinPath("", "b/"), "b/");
	EXPECT_EQ(JoinPath("a", "b/"), "a/b/");
	EXPECT_EQ(JoinPath("a/", "/b/"), "a/b/");
	EXPECT_EQ(JoinPath("a/", "b/"), "a/b/");
	EXPECT_EQ(JoinPath("a", "/b/"), "a/b/");
}
