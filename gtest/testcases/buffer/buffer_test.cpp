#include "buffer.hpp"
#include "gtest.h"

TEST(read_buffer, basic_test)
{
	buffer::Buffer buf(3);

	EXPECT_TRUE(buf.GetChar().IsErr());
	EXPECT_TRUE(buf.GetChar().IsErr());
	EXPECT_FALSE(buf.push_back(std::vector<char>()).IsErr());
	EXPECT_FALSE(buf.push_back(std::vector<char>()).IsErr());
	EXPECT_FALSE(buf.push_back(std::vector<char>()).IsErr());
	EXPECT_FALSE(buf.push_back(std::vector<char>{'0'}).IsErr());
	EXPECT_FALSE(buf.push_back(std::vector<char>{'1', '2', '3'}).IsErr());
	EXPECT_FALSE(buf.push_back(std::vector<char>{'4', '5'}).IsErr());
	EXPECT_TRUE(buf.push_back(std::vector<char>{'6'}).IsErr());
	EXPECT_TRUE(buf.push_back(std::vector<char>{'6'}).IsErr());
	EXPECT_EQ(buf.GetChar().Val(), '0');
	EXPECT_EQ(buf.GetChar().Val(), '1');
	EXPECT_EQ(buf.GetChar().Val(), '2');
	EXPECT_EQ(buf.GetChar().Val(), '3');
	EXPECT_EQ(buf.GetChar().Val(), '4');
	EXPECT_EQ(buf.GetChar().Val(), '5');
	EXPECT_TRUE(buf.GetChar().IsErr());
}

TEST(read_buffer, get_all_test)
{
	buffer::Buffer buf(3);

	EXPECT_EQ(buf.GetAll(), std::vector<char>());
	EXPECT_EQ(buf.GetAll(), std::vector<char>());
	EXPECT_EQ(buf.GetAll(), std::vector<char>());
	buf.push_back(std::vector<char>{'0', '0'});
	buf.push_back(std::vector<char>{'1', '2', '3'});
	buf.push_back(std::vector<char>{'4', '5'});
	buf.GetChar();
	std::vector<char> v{'0', '1', '2', '3', '4', '5'};
	EXPECT_EQ(buf.GetAll(), v);
}

TEST(read_buffer, empty_test)
{
	buffer::Buffer buf(3);

	EXPECT_TRUE(buf.empty());
	EXPECT_TRUE(buf.empty());
	EXPECT_TRUE(buf.empty());
	buf.push_back(std::vector<char>{'a', 'b'});
	EXPECT_FALSE(buf.empty());
	EXPECT_FALSE(buf.empty());
	EXPECT_FALSE(buf.empty());
	buf.push_back(std::vector<char>{'c', 'd'});
	buf.push_back(std::vector<char>{'c', 'd'});
	buf.GetAll();
	EXPECT_TRUE(buf.empty());
	EXPECT_TRUE(buf.empty());
	EXPECT_TRUE(buf.empty());
}

TEST(read_buffer, is_full_test)
{
	buffer::Buffer buf(3);

	EXPECT_FALSE(buf.IsFull());
	EXPECT_FALSE(buf.IsFull());
	EXPECT_FALSE(buf.IsFull());
	buf.push_back(std::vector<char>{'a', 'b'});
	buf.push_back(std::vector<char>{'c', 'd'});
	buf.push_back(std::vector<char>{'c', 'd'});
	EXPECT_TRUE(buf.IsFull());
	EXPECT_TRUE(buf.IsFull());
	EXPECT_TRUE(buf.IsFull());
	EXPECT_TRUE(buf.IsFull());
}
