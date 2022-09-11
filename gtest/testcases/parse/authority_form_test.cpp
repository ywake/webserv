#include "authority_form.hpp"
#include "error.hpp"
#include "gtest.h"

// PORTがABNFに適合しないなど、AuthorityFormのメンバに対してのテストは行っていない。(authority_test.cppに既にあるため)
//ここではHTTP要件によってvalidになったり、invalidになったりするケースをテストする。

TEST(AuthorityFormTest, valid_authority_form)
{
	EXPECT_EQ(AuthorityForm("example.com:80"),  AuthorityForm(Authority("example.com:80")));
	EXPECT_EQ(AuthorityForm("example.com:"), AuthorityForm(Authority("example.com:")));
}

TEST(AuthorityFormTest, invalid_on_http)
{
	EXPECT_THROW(AuthorityForm(""), Error);
	EXPECT_THROW(AuthorityForm(":"), Error);
	EXPECT_THROW(AuthorityForm(":80"), Error);
}

TEST(AuthorityFormTest, invalid_authority_form)
{
	EXPECT_THROW(AuthorityForm("example.com"), Error);
	EXPECT_THROW(AuthorityForm("example.com80"), Error);
}
