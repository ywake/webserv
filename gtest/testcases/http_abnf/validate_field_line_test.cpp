#include "gtest.h"

#include "validate_field_line.hpp"

TEST(field_line, is_ows_true)
{
	EXPECT_TRUE(http_abnf::IsOws(""));
	EXPECT_TRUE(http_abnf::IsOws(" "));
	EXPECT_TRUE(http_abnf::IsOws("\t"));
	EXPECT_TRUE(http_abnf::IsOws("\t "));
	EXPECT_TRUE(http_abnf::IsOws(" \t "));
	EXPECT_TRUE(http_abnf::IsOws("\t \t"));
}

TEST(field_line, is_ows_false)
{
	EXPECT_FALSE(http_abnf::IsOws(" a"));
	EXPECT_FALSE(http_abnf::IsOws("\ta"));
	EXPECT_FALSE(http_abnf::IsOws("\t a"));
	EXPECT_FALSE(http_abnf::IsOws("\t \r"));
	EXPECT_FALSE(http_abnf::IsOws("\t \n"));
	EXPECT_FALSE(http_abnf::IsOws(" a"));
	EXPECT_FALSE(http_abnf::IsOws("  a"));
	EXPECT_FALSE(http_abnf::IsOws("  \r"));
	EXPECT_FALSE(http_abnf::IsOws("  \n"));
	EXPECT_FALSE(http_abnf::IsOws("\n"));
	EXPECT_FALSE(http_abnf::IsOws("\r"));
}

TEST(field_line, is_rws_true)
{
	EXPECT_TRUE(http_abnf::IsRws(" "));
	EXPECT_TRUE(http_abnf::IsRws("\t"));
	EXPECT_TRUE(http_abnf::IsRws("\t "));
	EXPECT_TRUE(http_abnf::IsRws(" \t "));
	EXPECT_TRUE(http_abnf::IsRws("\t \t"));
}

TEST(field_line, is_rws_false)
{
	EXPECT_FALSE(http_abnf::IsRws(""));
	EXPECT_FALSE(http_abnf::IsRws("\n"));
	EXPECT_FALSE(http_abnf::IsRws("\r"));
	EXPECT_FALSE(http_abnf::IsRws(" a"));
	EXPECT_FALSE(http_abnf::IsRws("\ta"));
	EXPECT_FALSE(http_abnf::IsRws("\t a"));
	EXPECT_FALSE(http_abnf::IsRws("\t \r"));
	EXPECT_FALSE(http_abnf::IsRws("\t \n"));
	EXPECT_FALSE(http_abnf::IsRws(" a"));
	EXPECT_FALSE(http_abnf::IsRws("  a"));
	EXPECT_FALSE(http_abnf::IsRws("  \r"));
	EXPECT_FALSE(http_abnf::IsRws("  \n"));
}

TEST(field_line, is_field_vchar)
{
	for (int c = 0x00; c <= 0xFF; c++) {
		// std::cout << "c = " << (unsigned char)c << std::endl;
		if ((0x21 <= c && c <= 0x7E) || (0x80 <= c && c <= 0xFF)) {
			EXPECT_TRUE(http_abnf::IsFieldVchar(c));
		} else {
			EXPECT_FALSE(http_abnf::IsFieldVchar(c));
		}
	}
}

TEST(field_line, start_with_obsFold_true)
{
	EXPECT_TRUE(http_abnf::StartWithObsFold("\r\n "));
	EXPECT_TRUE(http_abnf::StartWithObsFold("\r\n\t"));
	EXPECT_TRUE(http_abnf::StartWithObsFold("\r\n aaaa"));
	EXPECT_TRUE(http_abnf::StartWithObsFold("\r\n\taaaa"));
	EXPECT_TRUE(http_abnf::StartWithObsFold(" \r\n aaaa"));
	EXPECT_TRUE(http_abnf::StartWithObsFold("\t\r\n	aaaa"));
	EXPECT_TRUE(http_abnf::StartWithObsFold(" \t\r\n	aaaa"));
	EXPECT_TRUE(http_abnf::StartWithObsFold(" \t \r\n	aaaa"));
	EXPECT_TRUE(http_abnf::StartWithObsFold("\t  \t\r\n	aaaa"));
}

TEST(field_line, start_with_obsFold_false)
{
	EXPECT_FALSE(http_abnf::StartWithObsFold(""));
	EXPECT_FALSE(http_abnf::StartWithObsFold(" "));
	EXPECT_FALSE(http_abnf::StartWithObsFold("\t"));
	EXPECT_FALSE(http_abnf::StartWithObsFold("\t "));
	EXPECT_FALSE(http_abnf::StartWithObsFold(" \t "));
	EXPECT_FALSE(http_abnf::StartWithObsFold("\t \t"));
	EXPECT_FALSE(http_abnf::StartWithObsFold("\r\n"));
	EXPECT_FALSE(http_abnf::StartWithObsFold(" \r\n"));
	EXPECT_FALSE(http_abnf::StartWithObsFold("\t\r\n"));
	EXPECT_FALSE(http_abnf::StartWithObsFold("a\r\n "));
	EXPECT_FALSE(http_abnf::StartWithObsFold("a\r\n\t"));
	EXPECT_FALSE(http_abnf::StartWithObsFold("a \r\n\t"));
}

TEST(field_line, end_with_obsFold_true)
{

	EXPECT_TRUE(http_abnf::EndWithObsFold("\r\n "));
	EXPECT_TRUE(http_abnf::EndWithObsFold("\r\n\t"));
	EXPECT_TRUE(http_abnf::EndWithObsFold(" \r\n "));
	EXPECT_TRUE(http_abnf::EndWithObsFold(" \r\n\t"));

	EXPECT_TRUE(http_abnf::EndWithObsFold("\r\n\r\n "));
	EXPECT_TRUE(http_abnf::EndWithObsFold("\r\n\r\n\t"));
	EXPECT_TRUE(http_abnf::EndWithObsFold("\r\n \r\n "));
	EXPECT_TRUE(http_abnf::EndWithObsFold("\r\n \r\n\t"));

	EXPECT_TRUE(http_abnf::EndWithObsFold("a\r\n "));
	EXPECT_TRUE(http_abnf::EndWithObsFold("a\r\n\t"));
	EXPECT_TRUE(http_abnf::EndWithObsFold("a \r\n\t"));
}

TEST(field_line, end_with_obsFold_false)
{
	EXPECT_FALSE(http_abnf::EndWithObsFold(""));
	EXPECT_FALSE(http_abnf::EndWithObsFold(" "));
	EXPECT_FALSE(http_abnf::EndWithObsFold("\t"));
	EXPECT_FALSE(http_abnf::EndWithObsFold("\t "));
	EXPECT_FALSE(http_abnf::EndWithObsFold(" \t "));
	EXPECT_FALSE(http_abnf::EndWithObsFold("\t \t"));
	EXPECT_FALSE(http_abnf::EndWithObsFold("\r\n"));
	EXPECT_FALSE(http_abnf::EndWithObsFold(" \r\n"));
	EXPECT_FALSE(http_abnf::EndWithObsFold("\t\r\n"));
	EXPECT_FALSE(http_abnf::EndWithObsFold("\r\n aaaa"));
	EXPECT_FALSE(http_abnf::EndWithObsFold("\r\n\taaaa"));
	EXPECT_FALSE(http_abnf::EndWithObsFold(" \r\n aaaa"));
	EXPECT_FALSE(http_abnf::EndWithObsFold("\t\r\n	aaaa"));
	EXPECT_FALSE(http_abnf::EndWithObsFold(" \t\r\n	aaaa"));
	EXPECT_FALSE(http_abnf::EndWithObsFold(" \t \r\n	aaaa"));
	EXPECT_FALSE(http_abnf::EndWithObsFold("\t  \t\r\n	aaaa"));
}
