#include "gtest.h"

#include "validate_headers.hpp"

#include "error.hpp"
#include "field_lines.hpp"

// #element => [element *( OWS "," OWS element )]
// Transfer-Encoding
// 			 = #transfer-coding
// t-codings = "trailers"
// 			 / ( transfer-coding [ weight ] )
// transfer-coding
// 			 = token *( OWS ";" OWS transfer-parameter )
// transfer-parameter
// 			 = token BWS "=" BWS ( token / quoted-string )
// weight	 = OWS ";" OWS "q=" qvalue
// qvalue	 = ( "0" [ "." 0*3DIGIT ] )
// 			 / ( "1" [ "." 0*3("0") ] )
// 規定の重みは 1.0 である。
TEST(http_headers, is_valid_transfer_encoding)
{
	EXPECT_TRUE(http_headers::IsValidTransferEncoding(FieldLines("")));
	EXPECT_TRUE(http_headers::IsValidTransferEncoding(FieldLines("Transfer-Encoding:chunked\r\n")));
	EXPECT_TRUE(http_headers::IsValidTransferEncoding(FieldLines("Transfer-Encoding:gzip\r\n"
																 "Transfer-Encoding:chunked\r\n")));
	EXPECT_TRUE(http_headers::IsValidTransferEncoding(FieldLines("Transfer-Encoding:chunked, \r\n"))
	);
	EXPECT_TRUE(http_headers::IsValidTransferEncoding(FieldLines("Transfer-Encoding:chunked,,,,\r\n"
	)));
	EXPECT_TRUE(
		http_headers::IsValidTransferEncoding(FieldLines("Transfer-Encoding:chunked, , , ,\r\n"))
	);
	EXPECT_TRUE(http_headers::IsValidTransferEncoding(FieldLines("Transfer-Encoding:, chunked, \r\n"
	)));
	EXPECT_TRUE(http_headers::IsValidTransferEncoding(FieldLines("Transfer-Encoding:,chunked,\r\n"))
	);
	EXPECT_TRUE(http_headers::IsValidTransferEncoding(FieldLines("Transfer-Encoding:\r\n")));
	EXPECT_TRUE(http_headers::IsValidTransferEncoding(FieldLines("Transfer-Encoding: \r\n")));

	EXPECT_TRUE(http_headers::IsValidTransferEncoding(FieldLines("Transfer-Encoding: chunked\r\n"))
	);

	// チャンク化転送符号法を複数回適用してはならない（送信者）受信者は最後chunkedならok？
	EXPECT_TRUE(http_headers::IsValidTransferEncoding(FieldLines("Transfer-Encoding: chunked\r\n"
																 "Transfer-Encoding: chunked\r\n"))
	);
	EXPECT_TRUE(
		http_headers::IsValidTransferEncoding(FieldLines("Transfer-Encoding: chunked, chunked\r\n"))
	);
	EXPECT_TRUE(http_headers::IsValidTransferEncoding(
		FieldLines("Transfer-Encoding: chunked, gzip, chunked\r\n")
	));

	// チャンク化以外の転送符号法が適用される場合、チャンク化を最終転送符号法としなければならない
	EXPECT_TRUE(
		http_headers::IsValidTransferEncoding(FieldLines("Transfer-Encoding: gzip, chunked\r\n"))
	);
	EXPECT_FALSE(
		http_headers::IsValidTransferEncoding(FieldLines("Transfer-Encoding: chunked, gzip\r\n"))
	);

	EXPECT_FALSE(http_headers::IsValidTransferEncoding(FieldLines("Transfer-Encoding: chunked, \r\n"
	)));

	// 不明な転送符号法を伴うリクエストには501でレスポンス
	EXPECT_THROW(
		http_headers::IsValidTransferEncoding(FieldLines("Transfer-Encoding: unknown\r\n")), Error
	);
	EXPECT_THROW(
		http_headers::IsValidTransferEncoding(FieldLines("Transfer-Encoding: trailers\r\n")), Error
	);
	EXPECT_THROW(
		http_headers::IsValidTransferEncoding(FieldLines("Transfer-Encoding: gzip\r\n")), Error
	);
}
