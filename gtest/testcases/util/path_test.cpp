#include "gtest.h"
#include "result.hpp"
#include "webserv_utils.hpp"

using namespace utils;

#define RESULT(val) Result<std::string>(std::string(val))

TEST(normalize_path, http_normalize_path)
{
	EXPECT_EQ(HttpNormalizePath(""), RESULT("/"));
	EXPECT_EQ(HttpNormalizePath("/"), RESULT("/"));
	EXPECT_EQ(HttpNormalizePath("/a/"), RESULT("/a/"));
	EXPECT_EQ(HttpNormalizePath("a"), RESULT("/a"));
	EXPECT_EQ(HttpNormalizePath("/a"), RESULT("/a"));
	EXPECT_EQ(HttpNormalizePath("a/.."), RESULT("/"));
	EXPECT_EQ(HttpNormalizePath("a/../"), RESULT("/"));
	EXPECT_EQ(HttpNormalizePath("/a/.."), RESULT("/"));
	EXPECT_EQ(HttpNormalizePath("/a/../"), RESULT("/"));
	EXPECT_EQ(HttpNormalizePath("a/."), RESULT("/a/"));
	EXPECT_EQ(HttpNormalizePath("a/./"), RESULT("/a/"));
	EXPECT_EQ(HttpNormalizePath("/a/."), RESULT("/a/"));
	EXPECT_EQ(HttpNormalizePath("/a/./"), RESULT("/a/"));
	EXPECT_EQ(HttpNormalizePath("./"), RESULT("/"));
	EXPECT_EQ(HttpNormalizePath("a//"), RESULT("/a/"));
	EXPECT_EQ(HttpNormalizePath("..a"), RESULT("/..a"));
	EXPECT_EQ(HttpNormalizePath("..a/"), RESULT("/..a/"));
	EXPECT_EQ(HttpNormalizePath("a.."), RESULT("/a.."));
	EXPECT_EQ(HttpNormalizePath("/a.."), RESULT("/a.."));
	EXPECT_EQ(HttpNormalizePath("a../"), RESULT("/a../"));
	EXPECT_EQ(HttpNormalizePath(".a"), RESULT("/.a"));
	EXPECT_EQ(HttpNormalizePath(".a/"), RESULT("/.a/"));
	EXPECT_EQ(HttpNormalizePath("a."), RESULT("/a."));
	EXPECT_EQ(HttpNormalizePath("/a."), RESULT("/a."));
	EXPECT_EQ(HttpNormalizePath("a./"), RESULT("/a./"));
	EXPECT_EQ(HttpNormalizePath("/a/b/c/./../../g"), RESULT("/a/g"));

	EXPECT_EQ(HttpNormalizePath("../a/./../.././b/./../"), Error("Invalid path"));
	EXPECT_EQ(HttpNormalizePath("../../a/../b/c/.././d/../"), Error("Invalid path"));
}

TEST(normalize_path, normalize_path)
{
	EXPECT_EQ(NormalizePath(""), RESULT(""));
	EXPECT_EQ(NormalizePath("/"), RESULT("/"));
	EXPECT_EQ(NormalizePath("/a/"), RESULT("/a/"));
	EXPECT_EQ(NormalizePath("a"), RESULT("a"));
	EXPECT_EQ(NormalizePath("/a"), RESULT("/a"));
	EXPECT_EQ(NormalizePath("a/.."), RESULT(""));
	EXPECT_EQ(NormalizePath("a/../"), RESULT(""));
	EXPECT_EQ(NormalizePath("/a/.."), RESULT("/"));
	EXPECT_EQ(NormalizePath("/a/../"), RESULT("/"));
	EXPECT_EQ(NormalizePath("a/."), RESULT("a/"));
	EXPECT_EQ(NormalizePath("a/./"), RESULT("a/"));
	EXPECT_EQ(NormalizePath("/a/."), RESULT("/a/"));
	EXPECT_EQ(NormalizePath("/a/./"), RESULT("/a/"));
	EXPECT_EQ(NormalizePath("./"), RESULT(""));
	EXPECT_EQ(NormalizePath("a//"), RESULT("a/"));
	EXPECT_EQ(NormalizePath("..a"), RESULT("..a"));
	EXPECT_EQ(NormalizePath("..a/"), RESULT("..a/"));
	EXPECT_EQ(NormalizePath("a.."), RESULT("a.."));
	EXPECT_EQ(NormalizePath("/a.."), RESULT("/a.."));
	EXPECT_EQ(NormalizePath("a../"), RESULT("a../"));
	EXPECT_EQ(NormalizePath(".a"), RESULT(".a"));
	EXPECT_EQ(NormalizePath(".a/"), RESULT(".a/"));
	EXPECT_EQ(NormalizePath("a."), RESULT("a."));
	EXPECT_EQ(NormalizePath("/a."), RESULT("/a."));
	EXPECT_EQ(NormalizePath("a./"), RESULT("a./"));
	EXPECT_EQ(NormalizePath("/a/b/c/./../../g"), RESULT("/a/g"));

	EXPECT_EQ(NormalizePath("../a/./../.././b/./../"), Error("Invalid path"));
	EXPECT_EQ(NormalizePath("../../a/../b/c/.././d/../"), Error("Invalid path"));
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
