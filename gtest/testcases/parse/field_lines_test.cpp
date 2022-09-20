#include "gtest.h"

#include <list>

#include "field_lines.hpp"

TEST(FieldLines, TokenizeLines)
{
	FieldLines field_lines;

	EXPECT_EQ(
		field_lines.TokenizeLines("key: value\r\n"
								  "key2: value2\r\n"
								  " value2\r\n"
								  "\tvalue2\r\n"),
		std::list<FieldLines::Token>(
			{{FieldLines::Token("key: value", FieldLines::kFieldLineTk),
			  FieldLines::Token("\r\n", FieldLines::kCrLfTk),
			  FieldLines::Token(
				  "key2: value2\r\n"
				  " value2\r\n"
				  "\tvalue2",
				  FieldLines::kFieldLineTk
			  ),
			  FieldLines::Token("\r\n", FieldLines::kCrLfTk)}}
		)
	);
	EXPECT_EQ(
		field_lines.TokenizeLines("\r\n"),
		std::list<FieldLines::Token>({FieldLines::Token("\r\n", FieldLines::kCrLfTk)})
	);
	EXPECT_EQ(
		field_lines.TokenizeLines("\r\n "),
		std::list<FieldLines::Token>({FieldLines::Token("\r\n ", FieldLines::kFieldLineTk)})
	);
	EXPECT_EQ(
		field_lines.TokenizeLines("key: value\r\n"
								  "\r\n"
								  "\r\n"),
		std::list<FieldLines::Token>(
			{FieldLines::Token("key: value", FieldLines::kFieldLineTk),
			 FieldLines::Token("\r\n", FieldLines::kCrLfTk),
			 FieldLines::Token("\r\n", FieldLines::kCrLfTk),
			 FieldLines::Token("\r\n", FieldLines::kCrLfTk)}
		)
	);
	EXPECT_EQ(
		field_lines.TokenizeLines("key\r\n"
								  " : value\r\n"),
		std::list<FieldLines::Token>(
			{FieldLines::Token("key\r\n : value", FieldLines::kFieldLineTk),
			 FieldLines::Token("\r\n", FieldLines::kCrLfTk)}
		)
	);
	EXPECT_EQ(
		field_lines.TokenizeLines("key\r\n"
								  "\t: value\r\n"),
		std::list<FieldLines::Token>(
			{FieldLines::Token("key\r\n\t: value", FieldLines::kFieldLineTk),
			 FieldLines::Token("\r\n", FieldLines::kCrLfTk)}
		)
	);
	EXPECT_EQ(
		field_lines.TokenizeLines("key\r\n"
								  " : value"),
		std::list<FieldLines::Token>(
			{FieldLines::Token("key\r\n : value", FieldLines::kFieldLineTk)}
		)
	);
	EXPECT_EQ(
		field_lines.TokenizeLines("key\r\n"
								  "\t: value"),
		std::list<FieldLines::Token>(
			{FieldLines::Token("key\r\n\t: value", FieldLines::kFieldLineTk)}
		)
	);
	EXPECT_EQ(
		field_lines.TokenizeLines("key: value\r\n"
								  " \r\n"
								  "\t\r\n"),
		std::list<FieldLines::Token>(
			{FieldLines::Token(
				 "key: value\r\n"
				 " \r\n"
				 "\t",
				 FieldLines::kFieldLineTk
			 ),
			 FieldLines::Token("\r\n", FieldLines::kCrLfTk)}
		)
	);
	EXPECT_EQ(
		field_lines.TokenizeLines("key: value\r\n"
								  " \r\n"
								  "\t"),
		std::list<FieldLines::Token>({
			FieldLines::Token(
				"key: value\r\n"
				" \r\n"
				"\t",
				FieldLines::kFieldLineTk
			),
		})
	);
}
