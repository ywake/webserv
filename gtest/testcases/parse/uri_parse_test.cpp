#include "error.hpp"
#include "gtest.h"
#include "request_target.hpp"
#include "result.hpp"
#include "uri.hpp"

static Result<RequestTarget> test_actualy(std::string input)
{
	RequestTarget act;
	Error err;
	try {
		act = RequestTarget(input);
	} catch (const Error &e) {
		err = e;
	}
	return Result<RequestTarget>(act, err);
}

TEST(uri_parse, empty)
{
	Result<RequestTarget> act = test_actualy("");
	ASSERT_EQ(act.err, Error("400"));
}

TEST(uri_parse, specify_form)
{
	{
		std::string input = "/where?q=now";
		RequestTarget req;
		req.SpecifyForm(input);
		ASSERT_EQ(req.form_type_, RequestTarget::ORIGIN);
	}

	{
		std::string input = "http://www.example.org/pub/WWW/TheProject.html";
		RequestTarget req;
		req.SpecifyForm(input);
		ASSERT_EQ(req.form_type_, RequestTarget::ABSOLUTE);
	}

	{
		std::string input = "www.example.com:80";
		RequestTarget req;
		req.SpecifyForm(input);
		ASSERT_EQ(req.form_type_, RequestTarget::AUTHORITY);
	}

	{
		std::string input = "*";
		RequestTarget req;
		req.SpecifyForm(input);
		ASSERT_EQ(req.form_type_, RequestTarget::ASTERISK);
	}
}

#include <iostream>

void test_parse_origin_form(const std::string &input, const URI &uri, const Error &err = Error())
{
	RequestTarget exp(RequestTarget::ORIGIN, uri);
	Result<RequestTarget> act = test_actualy(input);
	if (act.Val() != exp) {
		std::cout << "***Expect***" << std::endl;
		std::cout << exp.request_target_ << std::endl;
		std::cout << "***Actual***" << std::endl;
		std::cout << act.Val().request_target_ << std::endl;
	}
	if (act.err != err) {
		std::cout << "***Error***" << std::endl;
		std::cout << act.Err() << " : " << err.Err() << std::endl;
	}
	EXPECT_EQ(act.Val(), exp);
	EXPECT_EQ(act.err, err);
}

TEST(uri_parse, parse_origin_form)
{
	test_parse_origin_form("/", URI("", "", "", "", "/"));
	test_parse_origin_form("/?", URI("", "", "", "", "/", "?"));
	test_parse_origin_form("/where", URI("", "", "", "", "/where"));
	test_parse_origin_form("/where?q=now", URI("", "", "", "", "/where", "?q=now"));
	test_parse_origin_form("/where/is/this?q=now", URI("", "", "", "", "/where/is/this", "?q=now"));
	test_parse_origin_form(
		"/where/is/this?q=now???", URI("", "", "", "", "/where/is/this", "?q=now???")
	);
	test_parse_origin_form("//", URI("", "", "", "", "//"));
	test_parse_origin_form("/a//", URI("", "", "", "", "/a//"));
	test_parse_origin_form("///", URI("", "", "", "", "///"));
	test_parse_origin_form("/a///", URI("", "", "", "", "/a///"));
	test_parse_origin_form("/where/is/this///", URI("", "", "", "", "/where/is/this///"));

	// Error Case
	test_parse_origin_form("", URI(), Error("400"));
	test_parse_origin_form("/where/is/this>", URI(), Error("400"));
	test_parse_origin_form("/where?#", URI(), Error("400"));
	test_parse_origin_form("/where???#", URI(), Error("400"));
}

// TODO:
TEST(uri_parse, minimum_absolute_target)
{
	// Result<RequestTarget> act = test_actualy("a://");
	// URI exp("a", "", "", "", "", "", "");
	//
	// ASSERT_EQ(act.Val(), exp);
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
