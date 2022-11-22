#include "buffer.hpp"
#include "gtest.h"

TEST(read_buffer, basic_test)
{
	buffer::Buffer buf;

	EXPECT_TRUE(buf.PopChar().empty());
	EXPECT_TRUE(buf.PopChar().empty());
	buf.push_back(std::vector<char>());
	buf.push_back(std::vector<char>());
	buf.push_back(std::vector<char>());
	EXPECT_TRUE(buf.empty());
	EXPECT_TRUE(buf.empty());
	buf.push_back(std::vector<char>{'0'});
	buf.push_back(std::vector<char>{'1', '2', '3'});
	buf.push_back(std::vector<char>{'4', '5'});
	buf.push_back(std::vector<char>{'6'});
	buf.push_back(std::vector<char>{'6'});
	EXPECT_EQ(buf.PopChar().Value(), '0');
	EXPECT_EQ(buf.PopChar().Value(), '1');
	EXPECT_EQ(buf.PopChar().Value(), '2');
	EXPECT_EQ(buf.PopChar().Value(), '3');
	EXPECT_EQ(buf.PopChar().Value(), '4');
	EXPECT_EQ(buf.PopChar().Value(), '5');
	EXPECT_EQ(buf.PopChar().Value(), '6');
	EXPECT_EQ(buf.PopChar().Value(), '6');
	EXPECT_TRUE(buf.PopChar().empty());
}

TEST(read_buffer, get_all_test)
{
	buffer::Buffer buf;

	EXPECT_EQ(buf.PopAll(), std::vector<char>());
	EXPECT_EQ(buf.PopAll(), std::vector<char>());
	EXPECT_EQ(buf.PopAll(), std::vector<char>());
	buf.push_back(std::vector<char>{'0', '0'});
	buf.push_back(std::vector<char>{'1', '2', '3'});
	buf.push_back(std::vector<char>{'4', '5'});
	buf.PopChar();
	std::vector<char> v{'0', '1', '2', '3', '4', '5'};
	EXPECT_EQ(buf.PopAll(), v);
}

TEST(read_buffer, empty_test)
{
	buffer::Buffer buf;

	EXPECT_TRUE(buf.empty());
	EXPECT_TRUE(buf.empty());
	EXPECT_TRUE(buf.empty());
	buf.push_back(std::vector<char>{'a', 'b'});
	EXPECT_FALSE(buf.empty());
	EXPECT_FALSE(buf.empty());
	EXPECT_FALSE(buf.empty());
	buf.push_back(std::vector<char>{'c', 'd'});
	buf.push_back(std::vector<char>{'c', 'd'});
	buf.PopAll();
	EXPECT_TRUE(buf.empty());
	EXPECT_TRUE(buf.empty());
	EXPECT_TRUE(buf.empty());
}
