#include "gtest.h"

#include <list>

#include "field_line.hpp"

TEST(FieldLiness, TokenizeLines)
{
	FieldLiness field_lines;

	EXPECT_EQ(
		field_lines.TokenizeLines("key: value\r\n"
								  "key2: value2\r\n"
								  " value2\r\n"
								  "\tvalue2\r\n"),
		std::list<FieldLiness::Token>(
			{{FieldLiness::Token("key: value", FieldLiness::kFieldLineTk),
			  FieldLiness::Token("\r\n", FieldLiness::kCrLfTk),
			  FieldLiness::Token(
				  "key2: value2\r\n"
				  " value2\r\n"
				  "\tvalue2",
				  FieldLiness::kFieldLineTk
			  ),
			  FieldLiness::Token("\r\n", FieldLiness::kCrLfTk)}}
		)
	);
	EXPECT_EQ(
		field_lines.TokenizeLines("\r\n"),
		std::list<FieldLiness::Token>({FieldLiness::Token("\r\n", FieldLiness::kCrLfTk)})
	);
	EXPECT_EQ(
		field_lines.TokenizeLines("\r\n "),
		std::list<FieldLiness::Token>({FieldLiness::Token("\r\n ", FieldLiness::kFieldLineTk)})
	);
	EXPECT_EQ(
		field_lines.TokenizeLines("key: value\r\n"
								  "\r\n"
								  "\r\n"),
		std::list<FieldLiness::Token>(
			{FieldLiness::Token("key: value", FieldLiness::kFieldLineTk),
			 FieldLiness::Token("\r\n", FieldLiness::kCrLfTk),
			 FieldLiness::Token("\r\n", FieldLiness::kCrLfTk),
			 FieldLiness::Token("\r\n", FieldLiness::kCrLfTk)}
		)
	);
	EXPECT_EQ(
		field_lines.TokenizeLines("key\r\n"
								  " : value\r\n"),
		std::list<FieldLiness::Token>(
			{FieldLiness::Token("key\r\n : value", FieldLiness::kFieldLineTk),
			 FieldLiness::Token("\r\n", FieldLiness::kCrLfTk)}
		)
	);
	EXPECT_EQ(
		field_lines.TokenizeLines("key\r\n"
								  "\t: value\r\n"),
		std::list<FieldLiness::Token>(
			{FieldLiness::Token("key\r\n\t: value", FieldLiness::kFieldLineTk),
			 FieldLiness::Token("\r\n", FieldLiness::kCrLfTk)}
		)
	);
	EXPECT_EQ(
		field_lines.TokenizeLines("key\r\n"
								  " : value"),
		std::list<FieldLiness::Token>(
			{FieldLiness::Token("key\r\n : value", FieldLiness::kFieldLineTk)}
		)
	);
	EXPECT_EQ(
		field_lines.TokenizeLines("key\r\n"
								  "\t: value"),
		std::list<FieldLiness::Token>(
			{FieldLiness::Token("key\r\n\t: value", FieldLiness::kFieldLineTk)}
		)
	);
	EXPECT_EQ(
		field_lines.TokenizeLines("key: value\r\n"
								  " \r\n"
								  "\t\r\n"),
		std::list<FieldLiness::Token>(
			{FieldLiness::Token(
				 "key: value\r\n"
				 " \r\n"
				 "\t",
				 FieldLiness::kFieldLineTk
			 ),
			 FieldLiness::Token("\r\n", FieldLiness::kCrLfTk)}
		)
	);
	EXPECT_EQ(
		field_lines.TokenizeLines("key: value\r\n"
								  " \r\n"
								  "\t"),
		std::list<FieldLiness::Token>({
			FieldLiness::Token(
				"key: value\r\n"
				" \r\n"
				"\t",
				FieldLiness::kFieldLineTk
			),
		})
	);
}
