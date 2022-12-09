#include "gtest.h"

#include "parse_chunked.hpp"

TEST(chunk_size, ok)
{
	EXPECT_EQ(Result<std::size_t>(0), http::ParseChunkSize("0"));
	EXPECT_EQ(Result<std::size_t>(0), http::ParseChunkSize("0;"));
	EXPECT_EQ(Result<std::size_t>(0), http::ParseChunkSize("0; "));
	EXPECT_EQ(Result<std::size_t>(0), http::ParseChunkSize("0 ;"));
	EXPECT_EQ(Result<std::size_t>(0), http::ParseChunkSize("0\t;"));
	EXPECT_EQ(Result<std::size_t>(0), http::ParseChunkSize("0;\t"));
	EXPECT_EQ(Result<std::size_t>(1), http::ParseChunkSize("1"));
	EXPECT_EQ(Result<std::size_t>(2), http::ParseChunkSize("2"));
	EXPECT_EQ(Result<std::size_t>(3), http::ParseChunkSize("3"));
	EXPECT_EQ(Result<std::size_t>(4), http::ParseChunkSize("4"));
	EXPECT_EQ(Result<std::size_t>(5), http::ParseChunkSize("5"));
	EXPECT_EQ(Result<std::size_t>(6), http::ParseChunkSize("6"));
	EXPECT_EQ(Result<std::size_t>(7), http::ParseChunkSize("7"));
	EXPECT_EQ(Result<std::size_t>(8), http::ParseChunkSize("8"));
	EXPECT_EQ(Result<std::size_t>(9), http::ParseChunkSize("9"));
	EXPECT_EQ(Result<std::size_t>(10), http::ParseChunkSize("a"));
	EXPECT_EQ(Result<std::size_t>(11), http::ParseChunkSize("b"));
	EXPECT_EQ(Result<std::size_t>(12), http::ParseChunkSize("c"));
	EXPECT_EQ(Result<std::size_t>(13), http::ParseChunkSize("d"));
	EXPECT_EQ(Result<std::size_t>(14), http::ParseChunkSize("e"));
	EXPECT_EQ(Result<std::size_t>(15), http::ParseChunkSize("f"));
	EXPECT_EQ(Result<std::size_t>(10), http::ParseChunkSize("A"));
	EXPECT_EQ(Result<std::size_t>(15), http::ParseChunkSize("F"));
	EXPECT_EQ(Result<std::size_t>(255), http::ParseChunkSize("FF"));
	EXPECT_EQ(Result<std::size_t>(255), http::ParseChunkSize("FF;"));
	EXPECT_EQ(Result<std::size_t>(255), http::ParseChunkSize("Ff;"));
}

TEST(chunk_size, error)
{
	EXPECT_TRUE(http::ParseChunkSize("").IsErr());
	EXPECT_TRUE(http::ParseChunkSize(";").IsErr());
	EXPECT_TRUE(http::ParseChunkSize(";1").IsErr());
	EXPECT_TRUE(http::ParseChunkSize(";;").IsErr());
	EXPECT_TRUE(http::ParseChunkSize("; ").IsErr());
	EXPECT_TRUE(http::ParseChunkSize("-1").IsErr());
	EXPECT_TRUE(http::ParseChunkSize("0x0").IsErr());
	EXPECT_TRUE(http::ParseChunkSize("0;\r").IsErr());
	EXPECT_TRUE(http::ParseChunkSize("0;\v").IsErr());
	EXPECT_TRUE(http::ParseChunkSize("0\r;").IsErr());
	EXPECT_TRUE(http::ParseChunkSize("0\n;").IsErr());
	EXPECT_TRUE(http::ParseChunkSize(" 0").IsErr());
	EXPECT_TRUE(http::ParseChunkSize(" 1").IsErr());
	EXPECT_TRUE(http::ParseChunkSize("\t1").IsErr());
	EXPECT_TRUE(http::ParseChunkSize("FFFFFFFFFFFFFFFFFFFFFFFFF").IsErr());
}
