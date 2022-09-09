#include "error.hpp"
#include "gtest.h"
#include "request_target.hpp"
#include "result.hpp"
#include "request_form_data.hpp"

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

void test_specify_form(const std::string &request_line, RequestTarget::RequestForm form)
{
	RequestTarget req;
	ASSERT_EQ(req.SpecifyForm(request_line), form);
}

TEST(uri_parse, specify_form)
{
	test_specify_form("/where?q=now", RequestTarget::ORIGIN);
	test_specify_form("http://www.example.org/pub/WWW/TheProject.html", RequestTarget::ABSOLUTE);
	test_specify_form("www.example.com:80", RequestTarget::AUTHORITY);
	test_specify_form("*", RequestTarget::ASTERISK);
}

void test_form(
	RequestTarget::RequestForm form,
	const std::string &input,
	const RequestFormData &uri,
	const Error &err = Error()
)
{
	RequestTarget exp(form, uri);
	Result<RequestTarget> act = test_actualy(input);
	EXPECT_EQ(act.Val(), exp);
	EXPECT_EQ(act.err, err);
}

TEST(uri_parse, parse_origin_form)
{
	test_form(RequestTarget::ORIGIN, "/", RequestFormData("", "", "", "", "/"));
	test_form(RequestTarget::ORIGIN, "/?", RequestFormData("", "", "", "", "/", ""));
	test_form(RequestTarget::ORIGIN, "/where", RequestFormData("", "", "", "", "/where"));
	test_form(
		RequestTarget::ORIGIN, "/where?q=now", RequestFormData("", "", "", "", "/where", "q=now")
	);
	test_form(
		RequestTarget::ORIGIN,
		"/where/is/this?q=now",
		RequestFormData("", "", "", "", "/where/is/this", "q=now")
	);
	test_form(
		RequestTarget::ORIGIN,
		"/where/is/this?q=now???",
		RequestFormData("", "", "", "", "/where/is/this", "q=now???")
	);
	test_form(RequestTarget::ORIGIN, "//", RequestFormData("", "", "", "", "//"));
	test_form(RequestTarget::ORIGIN, "/a//", RequestFormData("", "", "", "", "/a//"));
	test_form(RequestTarget::ORIGIN, "///", RequestFormData("", "", "", "", "///"));
	test_form(RequestTarget::ORIGIN, "/a///", RequestFormData("", "", "", "", "/a///"));
	test_form(
		RequestTarget::ORIGIN,
		"/where/is/this///",
		RequestFormData("", "", "", "", "/where/is/this///")
	);

	// Error Case
	test_form(RequestTarget::UNDEFINED, "", RequestFormData(), Error("400"));
	test_form(RequestTarget::UNDEFINED, "/where/is/this>", RequestFormData(), Error("400"));
	test_form(RequestTarget::UNDEFINED, "/where?#", RequestFormData(), Error("400"));
	test_form(RequestTarget::UNDEFINED, "/where???#", RequestFormData(), Error("400"));
}

TEST(uri_parse, absolute_form)
{
	test_form(RequestTarget::ABSOLUTE, "a://a", RequestFormData("a", "", "a"));

	test_form(RequestTarget::UNDEFINED, "a://", RequestFormData(), Error("400"));
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
