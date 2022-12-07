#include "gtest.h"
#include "webserv_utils.hpp"

using namespace utils;

TEST(normalize_path, ok)
{
	EXPECT_EQ(NromalizePath(""), "/");
	EXPECT_EQ(NromalizePath("/"), "/");
	EXPECT_EQ(NromalizePath("/a/"), "/a/");
	EXPECT_EQ(NromalizePath("a"), "/a");
	EXPECT_EQ(NromalizePath("/a"), "/a");
	EXPECT_EQ(NromalizePath("a/.."), "/");
	EXPECT_EQ(NromalizePath("a/../"), "/");
	EXPECT_EQ(NromalizePath("/a/.."), "/");
	EXPECT_EQ(NromalizePath("/a/../"), "/");
	EXPECT_EQ(NromalizePath("a/."), "/a/");
	EXPECT_EQ(NromalizePath("a/./"), "/a/");
	EXPECT_EQ(NromalizePath("/a/."), "/a/");
	EXPECT_EQ(NromalizePath("/a/./"), "/a/");
	EXPECT_EQ(NromalizePath("./"), "/");
	EXPECT_EQ(NromalizePath("a//"), "/a/");
	EXPECT_EQ(NromalizePath("..a"), "/..a");
	EXPECT_EQ(NromalizePath("..a/"), "/..a/");
	EXPECT_EQ(NromalizePath("a.."), "/a..");
	EXPECT_EQ(NromalizePath("/a.."), "/a..");
	EXPECT_EQ(NromalizePath("a../"), "/a../");
	EXPECT_EQ(NromalizePath(".a"), "/.a");
	EXPECT_EQ(NromalizePath(".a/"), "/.a/");
	EXPECT_EQ(NromalizePath("a."), "/a.");
	EXPECT_EQ(NromalizePath("/a."), "/a.");
	EXPECT_EQ(NromalizePath("a./"), "/a./");
	EXPECT_EQ(NromalizePath("../a/./../.././b/./../"), "/");
	EXPECT_EQ(NromalizePath("../../a/../b/c/.././d/../"), "/b/");
	EXPECT_EQ(NromalizePath("/a/b/c/./../../g"), "/a/g");
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
