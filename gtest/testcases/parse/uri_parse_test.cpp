#include "gtest.h"

#include "error.hpp"
#include "request_form_data.hpp"
#include "request_target.hpp"
#include "result.hpp"

// RequestLineで空文字列は別分岐になるが、ここでもテストするか？
// TEST(uri_parse, empty)
// {
// 	ASSERT_THROW(RequestTarget(AbsoluteForm(""), Error("400"));
// }

// TEST(uri_parse, specify_form)
// {
// 	test_specify_form("/where?q=now");
// 	test_specify_form("http://www.example.org/pub/WWW/TheProject.html", RequestTarget::ABSOLUTE);
// 	test_specify_form("www.example.com:80", RequestTarget::AUTHORITY);
// 	test_specify_form("*", RequestTarget::ASTERISK);
// }

TEST(uri_parse, parse_origin_form)
{
	EXPECT_EQ(RequestTarget(OriginForm("/")), RequestTarget(RequestFormData("", "", "", "", "/")));
	EXPECT_EQ(
		RequestTarget(OriginForm("/?")), RequestTarget(RequestFormData("", "", "", "", "/", ""))
	);
	EXPECT_EQ(
		RequestTarget(OriginForm("/where")),
		RequestTarget(RequestFormData("", "", "", "", "/where"))
	);
	EXPECT_EQ(
		RequestTarget(OriginForm("/where?q=now")),
		RequestTarget(RequestFormData("", "", "", "", "/where", "q=now"))
	);
	EXPECT_EQ(
		RequestTarget(OriginForm("/where/is/this?q=now")),
		RequestTarget(RequestFormData("", "", "", "", "/where/is/this", "q=now"))
	);
	EXPECT_EQ(
		RequestTarget(OriginForm("/where/is/this?q=now???")),
		RequestTarget(RequestFormData("", "", "", "", "/where/is/this", "q=now???"))
	);
	EXPECT_EQ(
		RequestTarget(OriginForm("//")), RequestTarget(RequestFormData("", "", "", "", "//"))
	);
	EXPECT_EQ(
		RequestTarget(OriginForm("/a//")), RequestTarget(RequestFormData("", "", "", "", "/a//"))
	);
	EXPECT_EQ(
		RequestTarget(OriginForm("///")), RequestTarget(RequestFormData("", "", "", "", "///"))
	);
	EXPECT_EQ(
		RequestTarget(OriginForm("/a///")), RequestTarget(RequestFormData("", "", "", "", "/a///"))
	);
	EXPECT_EQ(
		RequestTarget(OriginForm("/where/is/this///")),
		RequestTarget(RequestFormData("", "", "", "", "/where/is/this///"))
	);

	// Error Case
	EXPECT_THROW(RequestTarget(OriginForm("")), Error);
	EXPECT_THROW(RequestTarget(OriginForm("/where/is/this>")), Error);
	EXPECT_THROW(RequestTarget(OriginForm("/where?#")), Error);
	EXPECT_THROW(RequestTarget(OriginForm("/where???#")), Error);
}

TEST(uri_parse, absolute_form)
{
	EXPECT_EQ(
		RequestTarget(AbsoluteForm("http://a")), RequestTarget(RequestFormData("http", "", "a"))
	);

	EXPECT_THROW(RequestTarget(AbsoluteForm("a://a")), Error);	 // scheme
	EXPECT_THROW(RequestTarget(AbsoluteForm("http://")), Error); // no-host
}

// a://
// scheme://username:password@test.abcdedgh.www.secondleveldomain.topleveldomain:1234/hello/world/basename.extension?name=ferret#hash
// URI           = scheme ":" hier-part [ "?" query ] [ "#" fragment ]

// scheme        = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )

// hier-part     = ("//" authority path-abempty)
//              / path-absolute
//              / path-rootless
//              / path-empty

// authority     = [ userinfo "@" ] host [ ":" port ]
// userinfo      = *( unreserved / pct-encoded / sub-delims / ":" )
// host          = IP-literal / IPv4address / reg-name
// port          = *DIGIT

// IP-literal    = "[" ( IPv6address / IPvFuture  ) "]"

// IPvFuture     = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )

// IPv6address   =                            6( h16 ":" ) ls32
//              /                       "::" 5( h16 ":" ) ls32
//              / [               h16 ] "::" 4( h16 ":" ) ls32
//              / [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
//              / [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
//              / [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32
//              / [ *4( h16 ":" ) h16 ] "::"              ls32
//              / [ *5( h16 ":" ) h16 ] "::"              h16
//              / [ *6( h16 ":" ) h16 ] "::"

// h16           = 1*4HEXDIG
// ls32          = ( h16 ":" h16 ) / IPv4address
// IPv4address   = dec-octet "." dec-octet "." dec-octet "." dec-octet

// dec-octet     = DIGIT                // 0-9
//              / %x31-39 DIGIT         // 10-99
//              / "1" 2DIGIT            // 100-199
//              / "2" %x30-34 DIGIT     // 200-249
//              / "25" %x30-35          // 250-255

// path-abempty  = *( "/" segment )
// path-absolute = "/" [ segment-nz *( "/" segment ) ]
// path-rootless = segment-nz *( "/" segment )
// path-empty    = 0<pchar>

// segment       = *pchar
// segment-nz    = 1*pchar

// pchar         = unreserved / pct-encoded / sub-delims / ":" / "@"

// query         = *( pchar / "/" / "?" )

// fragment      = *( pchar / "/" / "?" )

// pct-encoded   = "%" HEXDIG HEXDIG

// unreserved    = ALPHA / DIGIT / "-" / "." / "_" / "~"
// reserved      = gen-delims / sub-delims
// gen-delims    = ":" / "/" / "?" / "#" / "[" / "]" / "@"
// sub-delims    = "!" / "$" / "&" / "'" / "(" / ")"
//              / "*" / "+" / "," / ";" / "="
