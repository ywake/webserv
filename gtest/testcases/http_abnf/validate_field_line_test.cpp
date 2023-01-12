#include "gtest.h"

#include "validate_field_line.hpp"

TEST(field_line, is_ows_true)
{
	EXPECT_TRUE(http::abnf::IsOws(""));
	EXPECT_TRUE(http::abnf::IsOws(" "));
	EXPECT_TRUE(http::abnf::IsOws("\t"));
	EXPECT_TRUE(http::abnf::IsOws("\t "));
	EXPECT_TRUE(http::abnf::IsOws(" \t "));
	EXPECT_TRUE(http::abnf::IsOws("\t \t"));
}

TEST(field_line, is_ows_false)
{
	EXPECT_FALSE(http::abnf::IsOws(" a"));
	EXPECT_FALSE(http::abnf::IsOws("\ta"));
	EXPECT_FALSE(http::abnf::IsOws("\t a"));
	EXPECT_FALSE(http::abnf::IsOws("\t \r"));
	EXPECT_FALSE(http::abnf::IsOws("\t \n"));
	EXPECT_FALSE(http::abnf::IsOws(" a"));
	EXPECT_FALSE(http::abnf::IsOws("  a"));
	EXPECT_FALSE(http::abnf::IsOws("  \r"));
	EXPECT_FALSE(http::abnf::IsOws("  \n"));
	EXPECT_FALSE(http::abnf::IsOws("\n"));
	EXPECT_FALSE(http::abnf::IsOws("\r"));
}

TEST(field_line, is_rws_true)
{
	EXPECT_TRUE(http::abnf::IsRws(" "));
	EXPECT_TRUE(http::abnf::IsRws("\t"));
	EXPECT_TRUE(http::abnf::IsRws("\t "));
	EXPECT_TRUE(http::abnf::IsRws(" \t "));
	EXPECT_TRUE(http::abnf::IsRws("\t \t"));
}

TEST(field_line, is_rws_false)
{
	EXPECT_FALSE(http::abnf::IsRws(""));
	EXPECT_FALSE(http::abnf::IsRws("\n"));
	EXPECT_FALSE(http::abnf::IsRws("\r"));
	EXPECT_FALSE(http::abnf::IsRws(" a"));
	EXPECT_FALSE(http::abnf::IsRws("\ta"));
	EXPECT_FALSE(http::abnf::IsRws("\t a"));
	EXPECT_FALSE(http::abnf::IsRws("\t \r"));
	EXPECT_FALSE(http::abnf::IsRws("\t \n"));
	EXPECT_FALSE(http::abnf::IsRws(" a"));
	EXPECT_FALSE(http::abnf::IsRws("  a"));
	EXPECT_FALSE(http::abnf::IsRws("  \r"));
	EXPECT_FALSE(http::abnf::IsRws("  \n"));
}

TEST(field_line, is_field_vchar)
{
	for (int c = 0x00; c <= 0xFF; c++) {
		// std::cout << "c = " << (unsigned char)c << std::endl;
		if ((0x21 <= c && c <= 0x7E) || (0x80 <= c && c <= 0xFF)) {
			EXPECT_TRUE(http::abnf::IsFieldVchar(c));
		} else {
			EXPECT_FALSE(http::abnf::IsFieldVchar(c));
		}
	}
}

TEST(field_line, start_with_obsFold_true)
{
	EXPECT_TRUE(http::abnf::StartWithObsFold("\r\n "));
	EXPECT_TRUE(http::abnf::StartWithObsFold("\r\n\t"));
	EXPECT_TRUE(http::abnf::StartWithObsFold("\r\n aaaa"));
	EXPECT_TRUE(http::abnf::StartWithObsFold("\r\n\taaaa"));
	EXPECT_TRUE(http::abnf::StartWithObsFold(" \r\n aaaa"));
	EXPECT_TRUE(http::abnf::StartWithObsFold("\t\r\n	aaaa"));
	EXPECT_TRUE(http::abnf::StartWithObsFold(" \t\r\n	aaaa"));
	EXPECT_TRUE(http::abnf::StartWithObsFold(" \t \r\n	aaaa"));
	EXPECT_TRUE(http::abnf::StartWithObsFold("\t  \t\r\n	aaaa"));
}

TEST(field_line, start_with_obsFold_false)
{
	EXPECT_FALSE(http::abnf::StartWithObsFold(""));
	EXPECT_FALSE(http::abnf::StartWithObsFold(" "));
	EXPECT_FALSE(http::abnf::StartWithObsFold("\t"));
	EXPECT_FALSE(http::abnf::StartWithObsFold("\t "));
	EXPECT_FALSE(http::abnf::StartWithObsFold(" \t "));
	EXPECT_FALSE(http::abnf::StartWithObsFold("\t \t"));
	EXPECT_FALSE(http::abnf::StartWithObsFold("\r\n"));
	EXPECT_FALSE(http::abnf::StartWithObsFold(" \r\n"));
	EXPECT_FALSE(http::abnf::StartWithObsFold("\t\r\n"));
	EXPECT_FALSE(http::abnf::StartWithObsFold("a\r\n "));
	EXPECT_FALSE(http::abnf::StartWithObsFold("a\r\n\t"));
	EXPECT_FALSE(http::abnf::StartWithObsFold("a \r\n\t"));
}

TEST(field_line, end_with_obsFold_true)
{

	EXPECT_TRUE(http::abnf::EndWithObsFold("\r\n "));
	EXPECT_TRUE(http::abnf::EndWithObsFold("\r\n\t"));
	EXPECT_TRUE(http::abnf::EndWithObsFold(" \r\n "));
	EXPECT_TRUE(http::abnf::EndWithObsFold(" \r\n\t"));

	EXPECT_TRUE(http::abnf::EndWithObsFold("\r\n\r\n "));
	EXPECT_TRUE(http::abnf::EndWithObsFold("\r\n\r\n\t"));
	EXPECT_TRUE(http::abnf::EndWithObsFold("\r\n \r\n "));
	EXPECT_TRUE(http::abnf::EndWithObsFold("\r\n \r\n\t"));

	EXPECT_TRUE(http::abnf::EndWithObsFold("a\r\n "));
	EXPECT_TRUE(http::abnf::EndWithObsFold("a\r\n\t"));
	EXPECT_TRUE(http::abnf::EndWithObsFold("a \r\n\t"));
}

TEST(field_line, end_with_obsFold_false)
{
	EXPECT_FALSE(http::abnf::EndWithObsFold(""));
	EXPECT_FALSE(http::abnf::EndWithObsFold(" "));
	EXPECT_FALSE(http::abnf::EndWithObsFold("\t"));
	EXPECT_FALSE(http::abnf::EndWithObsFold("\t "));
	EXPECT_FALSE(http::abnf::EndWithObsFold(" \t "));
	EXPECT_FALSE(http::abnf::EndWithObsFold("\t \t"));
	EXPECT_FALSE(http::abnf::EndWithObsFold("\r\n"));
	EXPECT_FALSE(http::abnf::EndWithObsFold(" \r\n"));
	EXPECT_FALSE(http::abnf::EndWithObsFold("\t\r\n"));
	EXPECT_FALSE(http::abnf::EndWithObsFold("\r\n aaaa"));
	EXPECT_FALSE(http::abnf::EndWithObsFold("\r\n\taaaa"));
	EXPECT_FALSE(http::abnf::EndWithObsFold(" \r\n aaaa"));
	EXPECT_FALSE(http::abnf::EndWithObsFold("\t\r\n	aaaa"));
	EXPECT_FALSE(http::abnf::EndWithObsFold(" \t\r\n	aaaa"));
	EXPECT_FALSE(http::abnf::EndWithObsFold(" \t \r\n	aaaa"));
	EXPECT_FALSE(http::abnf::EndWithObsFold("\t  \t\r\n	aaaa"));
}

TEST(field_line, trim_obs_fold_sp)
{
	// EXPECT_EQ(http::abnf::TrimObsFold(""), "");
	// EXPECT_EQ(http::abnf::TrimObsFold(" "), " ");
	// EXPECT_EQ(http::abnf::TrimObsFold("\r\n"), "\r\n");
	// EXPECT_EQ(http::abnf::TrimObsFold(" \r\n "), " ");
	// EXPECT_EQ(http::abnf::TrimObsFold("	\r\n "), " ");
	// std::cerr << http::abnf::TrimObsFold("	\r\n ").size() << std::endl;
	// exit(0);
	// EXPECT_EQ(http::abnf::TrimObsFold("\t\r\n\t"), " ");
	// EXPECT_EQ(http::abnf::TrimObsFold("\t \r\n\t "), " ");
	// EXPECT_EQ(http::abnf::TrimObsFold(" \t \r\n\t "), " ");
	EXPECT_EQ(http::abnf::TrimObsFold("host:a\r\n b"), "host:a b");
	EXPECT_EQ(http::abnf::TrimObsFold("host:a\r\n "), "host:a ");
	EXPECT_EQ(http::abnf::TrimObsFold("host:a\r\n  "), "host:a ");
	EXPECT_EQ(http::abnf::TrimObsFold("host:a\r\n b\r\n c"), "host:a b c");
	EXPECT_EQ(http::abnf::TrimObsFold("host:a\r\n b\r\n c\r\n "), "host:a b c ");
	EXPECT_EQ(http::abnf::TrimObsFold("host:a\r\n b\r\n c \r\n "), "host:a b c ");
	EXPECT_EQ(http::abnf::TrimObsFold("host:\r\n a"), "host: a");
	EXPECT_EQ(http::abnf::TrimObsFold("host: \r\n a"), "host: a");
	EXPECT_EQ(http::abnf::TrimObsFold("host:\r\na"), "host:\r\na");
	EXPECT_EQ(http::abnf::TrimObsFold("host: \r\na"), "host: \r\na");
	EXPECT_EQ(http::abnf::TrimObsFold("host\r\n:a"), "host\r\n:a");
	EXPECT_EQ(http::abnf::TrimObsFold("host\r\n :a"), "host\r\n :a");
	EXPECT_EQ(http::abnf::TrimObsFold("\r\n host:a"), "\r\n host:a");

	EXPECT_EQ(http::abnf::TrimObsFold("host:a \r\n b"), "host:a b");
	EXPECT_EQ(http::abnf::TrimObsFold("host:a \r\n "), "host:a ");
	EXPECT_EQ(http::abnf::TrimObsFold("host:a \r\n  "), "host:a ");
	EXPECT_EQ(http::abnf::TrimObsFold("host:a \r\n b  \r\n c"), "host:a b c");
	EXPECT_EQ(http::abnf::TrimObsFold("host:a \r\n b  \r\n c		\r\n "), "host:a b c ");
	EXPECT_EQ(http::abnf::TrimObsFold("host:a \r\n b 	\r\n 	c  	 \r\n "), "host:a b c ");
	EXPECT_EQ(http::abnf::TrimObsFold("host:\r\n a"), "host: a");
	EXPECT_EQ(http::abnf::TrimObsFold("host:	\r\n a"), "host: a");
	EXPECT_EQ(http::abnf::TrimObsFold("host:\r\na"), "host:\r\na");
	EXPECT_EQ(http::abnf::TrimObsFold("host: \r\na"), "host: \r\na");
	EXPECT_EQ(http::abnf::TrimObsFold("host\r\n:a"), "host\r\n:a");
	EXPECT_EQ(http::abnf::TrimObsFold("host\r\n :a"), "host\r\n :a");
	EXPECT_EQ(http::abnf::TrimObsFold("\r\n host:a"), "\r\n host:a");
}

TEST(field_line, trim_obs_fold_tab)
{
	EXPECT_EQ(http::abnf::TrimObsFold("host:a\r\n\tb"), "host:a b");
	EXPECT_EQ(http::abnf::TrimObsFold("host:a\r\n\t"), "host:a ");
	EXPECT_EQ(http::abnf::TrimObsFold("host:a\r\n\t "), "host:a ");
	EXPECT_EQ(http::abnf::TrimObsFold("host:a\r\n\tb\r\n\tc"), "host:a b c");
	EXPECT_EQ(http::abnf::TrimObsFold("host:a\r\n\tb\r\n\tc\r\n\t"), "host:a b c ");
	EXPECT_EQ(http::abnf::TrimObsFold("host:a\r\n\tb\r\n\tc \r\n\t"), "host:a b c ");
	EXPECT_EQ(http::abnf::TrimObsFold("host:\r\n\ta"), "host: a");
	EXPECT_EQ(http::abnf::TrimObsFold("host:\r\n\ta"), "host: a");
	EXPECT_EQ(http::abnf::TrimObsFold("host:\r\na"), "host:\r\na");
	EXPECT_EQ(http::abnf::TrimObsFold("host: \r\na"), "host: \r\na");
	EXPECT_EQ(http::abnf::TrimObsFold("host\r\n:a"), "host\r\n:a");
	EXPECT_EQ(http::abnf::TrimObsFold("host\r\n\t:a"), "host\r\n\t:a");

	EXPECT_EQ(http::abnf::TrimObsFold("host:a\r\n\tb"), "host:a b");
	EXPECT_EQ(http::abnf::TrimObsFold("host:a\r\n\t"), "host:a ");
	EXPECT_EQ(http::abnf::TrimObsFold("host:a\r\n\t "), "host:a ");
	EXPECT_EQ(http::abnf::TrimObsFold("host:a\r\n\tb\r\n\tc"), "host:a b c");
	EXPECT_EQ(http::abnf::TrimObsFold("host:a\r\n\tb\r\n\tc\r\n\t"), "host:a b c ");
	EXPECT_EQ(http::abnf::TrimObsFold("host:a\r\n\tb\r\n\tc \r\n\t"), "host:a b c ");
	EXPECT_EQ(http::abnf::TrimObsFold("host:\t\r\n\ta"), "host: a");
	EXPECT_EQ(http::abnf::TrimObsFold("host:\t\r\n\ta"), "host: a");
	EXPECT_EQ(http::abnf::TrimObsFold("host:\t\r\na"), "host:\t\r\na");
	EXPECT_EQ(http::abnf::TrimObsFold("host:\t \r\na"), "host:\t \r\na");
	EXPECT_EQ(http::abnf::TrimObsFold("host\r\n:a"), "host\r\n:a");
	EXPECT_EQ(http::abnf::TrimObsFold("host\r\n\t:a"), "host\r\n\t:a");
}
