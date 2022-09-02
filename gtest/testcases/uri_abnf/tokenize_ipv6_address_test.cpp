#include "gtest.h"
#include "uri_abnf.hpp"

void test_tokenize_ipv6_address(const ThinString& input, ABNF::StringAry exp) {
	ABNF::StringAry act = ABNF::TokenizeIPv6address(input);
	EXPECT_EQ(act, exp);
}

TEST(uri_abnf, tokenize_ipv6_address) {

	test_tokenize_ipv6_address("a::b", {"a", "::", "b"});
	test_tokenize_ipv6_address("a::b:", {"a", "::", "b", ":"});
	test_tokenize_ipv6_address("a::b::", {"a", "::", "b", "::"});
	test_tokenize_ipv6_address(":a::b", {":", "a", "::", "b"});
	test_tokenize_ipv6_address("::a::b", {"::", "a", "::", "b"});
	test_tokenize_ipv6_address(":a::b:", {":", "a", "::", "b", ":"});
	test_tokenize_ipv6_address("::a::b::", {"::", "a", "::", "b", "::"});
}

TEST(uri_abnf, tokenize_ipv6_address_special_case) {
	test_tokenize_ipv6_address("::", {"::"});
	test_tokenize_ipv6_address(":", {":"});
	test_tokenize_ipv6_address("", {});
	test_tokenize_ipv6_address(":::", {"::", ":"});
	test_tokenize_ipv6_address("::::", {"::", "::"});
}