#include "gtest.h"

#include "ThinString.hpp"

#include <iostream>

TEST(thin_string, find)
{
	ThinString str("0123456789ABCDEF", 3, 10);
	// std::cout << str << std::endl;
	EXPECT_EQ(str.find("345"), 0);
}