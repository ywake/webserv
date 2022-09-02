#include "gtest.h"

#include "parse_authority.hpp"

// *( unreserved / pct-encoded / sub-delims / ":" )
TEST(uri_abnf, is_userinfo_unreserved_tests)
{
	EXPECT_TRUE(ABNF::IsUserInfo(""));
	EXPECT_TRUE(ABNF::IsUserInfo("username"));
	EXPECT_TRUE(ABNF::IsUserInfo("user-name"));
	EXPECT_TRUE(ABNF::IsUserInfo("user.name"));
	EXPECT_TRUE(ABNF::IsUserInfo("user_name"));
	EXPECT_TRUE(ABNF::IsUserInfo("user~name"));

	EXPECT_FALSE(ABNF::IsUserInfo("user@name"));
	EXPECT_FALSE(ABNF::IsUserInfo("user/name"));
	EXPECT_FALSE(ABNF::IsUserInfo("user|name"));
}

TEST(uri_abnf, is_userinfo_pct_encoded_tests)
{
	EXPECT_TRUE(ABNF::IsUserInfo("usern%61me"));

	EXPECT_FALSE(ABNF::IsUserInfo("user%%name"));
}

TEST(uri_abnf, is_userinfo_sub_delims_tests)
{
	EXPECT_TRUE(ABNF::IsUserInfo("user!name"));
	EXPECT_TRUE(ABNF::IsUserInfo("user&name"));
	EXPECT_TRUE(ABNF::IsUserInfo("user(name)"));
	EXPECT_TRUE(ABNF::IsUserInfo("user+name"));
	EXPECT_TRUE(ABNF::IsUserInfo("!$&'()*+,;="));
}

TEST(uri_abnf, is_userinfo_colon_tests)
{
	EXPECT_TRUE(ABNF::IsUserInfo("username:pass"));
	EXPECT_TRUE(ABNF::IsUserInfo("username:pass:"));
	EXPECT_TRUE(ABNF::IsUserInfo("username::"));
	EXPECT_TRUE(ABNF::IsUserInfo("username:pass:xxx"));
	EXPECT_TRUE(ABNF::IsUserInfo(":username"));
	EXPECT_TRUE(ABNF::IsUserInfo("::"));
}
