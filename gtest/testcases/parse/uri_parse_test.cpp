#include "error.hpp"
#include "gtest.h"
#include "result.hpp"
#include "uri.hpp"

static Result<URI> test_actualy(std::string input)
{
	URI act;
	Error err;
	try {
		act = URI(input);
	} catch (const Error &e) {
		err = e;
	}
	return Result<URI>(act, err);
}

TEST(uri_parse, empty)
{
	Result<URI> act = test_actualy("");
	ASSERT_EQ(act.err, Error("400"));
}

// TODO:
TEST(uri_parse, minimum_absolute_target)
{
	Result<URI> act = test_actualy("a://");
	URI exp("a", "", "", "", "", "", "");

	ASSERT_EQ(act.Val(), exp);
}

// a://
// scheme://username:password@test.abcdedgh.www.secondleveldomain.topleveldomain:1234/hello/world/basename.extension?name=ferret#hash
// URI           = scheme ":" hier-part [ "?" query ] [ "#" fragment ]

// scheme        = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )

// hier-part     = "//" authority path-abempty
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
