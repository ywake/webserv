#include "gtest.h"

#include "validate_headers.hpp"

#include "field_lines.hpp"

TEST(http_headers, is_valid_content_length)
{
	EXPECT_TRUE(http_headers::IsValidContentLength(FieldLines::Values()));
	EXPECT_TRUE(http_headers::IsValidContentLength(FieldLines::Values({"0"})));
	EXPECT_TRUE(http_headers::IsValidContentLength(FieldLines::Values({"1"})));
	EXPECT_TRUE(http_headers::IsValidContentLength(FieldLines::Values({"01"})));
	EXPECT_TRUE(http_headers::IsValidContentLength(FieldLines::Values({"12345"})));
	EXPECT_TRUE(http_headers::IsValidContentLength(FieldLines::Values({"9223372036854775807"})));

	EXPECT_FALSE(http_headers::IsValidContentLength(FieldLines::Values({"1, "})));
	EXPECT_FALSE(http_headers::IsValidContentLength(FieldLines::Values({", 1"})));
	EXPECT_FALSE(http_headers::IsValidContentLength(FieldLines::Values({"1", "2"})));
	EXPECT_FALSE(http_headers::IsValidContentLength(FieldLines::Values({"0", "0"})));
	EXPECT_FALSE(http_headers::IsValidContentLength(FieldLines::Values({"a"})));
	EXPECT_FALSE(http_headers::IsValidContentLength(FieldLines::Values({"0a"})));
	EXPECT_FALSE(http_headers::IsValidContentLength(FieldLines::Values({"9223372036854775808"})));
}
