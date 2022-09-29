#include "authority_form.hpp"
#include "gtest.h"
#include "http_exceptions.hpp"

// PORTがABNFに適合しないなど、AuthorityFormのメンバに対してのテストは行っていない。(authority_test.cppに既にあるため)
//ここではHTTP要件によってvalidになったり、invalidになったりするケースをテストする。

TEST(authority_form_test, valid_authority_form)
{
	EXPECT_EQ(
		AuthorityForm("example.com:80"), AuthorityForm(http_abnf::HostPort("example.com:80"))
	);
	EXPECT_EQ(AuthorityForm("example.com:"), AuthorityForm(http_abnf::HostPort("example.com:")));
}

TEST(authority_form_test, invalid_on_http)
{
	EXPECT_THROW(AuthorityForm(""), http::BadRequestException);
	EXPECT_THROW(AuthorityForm(":"), http::BadRequestException);
	EXPECT_THROW(AuthorityForm(":80"), http::BadRequestException);
}

TEST(authority_form_test, invalid_authority_form)
{
	EXPECT_THROW(AuthorityForm("example.com"), http::BadRequestException);
	EXPECT_THROW(AuthorityForm("example.com80"), http::BadRequestException);
	EXPECT_THROW(AuthorityForm("a@a:80"), http::BadRequestException);
}
