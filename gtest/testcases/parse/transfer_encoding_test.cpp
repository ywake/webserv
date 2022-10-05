#include "gtest.h"

#include "validate_headers.hpp"

#include "error.hpp"
#include "header_section.hpp"
#include "http_exceptions.hpp"

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
TEST(validate_headers, is_valid_transfer_encoding)
{
	EXPECT_TRUE(http::headers::IsValidTransferEncoding(HeaderSection("")["transfer-encoding"]));
	EXPECT_TRUE(http::headers::IsValidTransferEncoding(HeaderSection("Transfer-Encoding:chunked\r\n"
	)["transfer-encoding"]));
	EXPECT_TRUE(http::headers::IsValidTransferEncoding(
		HeaderSection("Transfer-Encoding: chunked\r\n")["transfer-encoding"]
	));

	// parse error扱いなので parseのテストでチェックする
	// EXPECT_TRUE(http::headers::IsValidTransferEncoding(HeaderSection("Transfer-Encoding:\r\n"
	// )["transfer-encoding"]));
	// EXPECT_TRUE(http::headers::IsValidTransferEncoding(HeaderSection("Transfer-Encoding:
	// \r\n")));
	// EXPECT_TRUE(
	// 	http::headers::IsValidTransferEncoding(HeaderSection("Transfer-Encoding:chunked, \r\n"))
	// );
	// EXPECT_TRUE(
	// 	http::headers::IsValidTransferEncoding(HeaderSection("Transfer-Encoding:chunked,,,,\r\n"))
	// );
	// EXPECT_TRUE(http::headers::IsValidTransferEncoding(
	// 	HeaderSection("Transfer-Encoding:chunked, , , ,\r\n")
	// ));
	// EXPECT_TRUE(
	// 	http::headers::IsValidTransferEncoding(HeaderSection("Transfer-Encoding:, chunked, \r\n"))
	// );
	// EXPECT_TRUE(
	// 	http::headers::IsValidTransferEncoding(HeaderSection("Transfer-Encoding:,chunked,\r\n"))
	// );
}

// チャンク化転送符号法を複数回適用してはならない（送信者）受信者は最後chunkedならok？
TEST(validate_headers, must_not_apply_chunked_twice)
{
	EXPECT_FALSE(http::headers::IsValidTransferEncoding(
		HeaderSection("Transfer-Encoding: chunked\r\n"
					  "Transfer-Encoding: chunked\r\n")["transfer-encoding"]
	));
	EXPECT_FALSE(http::headers::IsValidTransferEncoding(
		HeaderSection("Transfer-Encoding: chunked, chunked\r\n")["transfer-encoding"]
	));

	EXPECT_FALSE(http::headers::IsValidTransferEncoding(
		HeaderSection("Transfer-Encoding: chunked, gzip, chunked\r\n")["transfer-encoding"]
	));
}

// チャンク化以外の転送符号法が適用される場合、チャンク化を最終転送符号法としなければならない
TEST(validate_headers, chunk_must_be_last)
{
	EXPECT_TRUE(http::headers::IsValidTransferEncoding(
		HeaderSection("Transfer-Encoding: chunked\r\n")["transfer-encoding"]
	));

	EXPECT_TRUE(http::headers::IsValidTransferEncoding(
		HeaderSection("Transfer-Encoding: gzip, chunked\r\n")["transfer-encoding"]
	));

	EXPECT_FALSE(http::headers::IsValidTransferEncoding(
		HeaderSection("Transfer-Encoding: chunked, gzip\r\n")["transfer-encoding"]
	));
}

TEST(validate_headers, unknown_transfer_coding)
{
	// 不明な転送符号法を伴うリクエストには501でレスポンス
	EXPECT_FALSE(http::headers::IsImplementedTransferCoding(
		HeaderSection("Transfer-Encoding: unknown, chunked\r\n")["transfer-encoding"]
	));
	EXPECT_FALSE(http::headers::IsImplementedTransferCoding(
		HeaderSection("Transfer-Encoding: trailers, chunked\r\n")["transfer-encoding"]
	));
	EXPECT_FALSE(http::headers::IsImplementedTransferCoding(
		HeaderSection("Transfer-Encoding: gzip, chunked\r\n")["transfer-encoding"]
	));
	EXPECT_TRUE(http::headers::IsImplementedTransferCoding(
		HeaderSection("Transfer-Encoding: chunked\r\n")["transfer-encoding"]
	));
	EXPECT_TRUE(http::headers::IsImplementedTransferCoding(
		HeaderSection("Transfer-Encoding: chunked, chunked\r\n")["transfer-encoding"]
	));
}
