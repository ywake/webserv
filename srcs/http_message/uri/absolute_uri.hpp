#ifndef ABSOLUTE_URI_HPP
#define ABSOLUTE_URI_HPP

#include <string>

#include "hier_part.hpp"
#include "i_target_form.hpp"
#include "thin_string.hpp"

class AbsoluteUri
{
  private:
	ThinString scheme_;
	HierPart   hier_part_;
	ThinString query_;

  public:
	AbsoluteUri(const ThinString &str);
	AbsoluteUri(const ThinString &scheme, const HierPart &hier_part, const ThinString &query);

  private:
	void TrySetScheme(const ThinString &scheme);
	void TrySetQuery(const ThinString &query);

  public:
	const ThinString &GetScheme() const;
	const HierPart   &GetHierPart() const;
	const ThinString &GetQuery() const;

	bool operator==(const AbsoluteUri &rhs) const;
	bool operator!=(const AbsoluteUri &rhs) const;
};

std::ostream &operator<<(std::ostream &os, const AbsoluteUri &uri);

#endif

/*
 absolute-URI  = scheme ":" hier-part [ "?" query ]

 scheme        = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )

 hier-part     = "//" authority path-abempty
			   / path-absolute
			   / path-rootless
			   / path-empty

 query         = *( pchar / "/" / "?" )

 authority     = [ userinfo "@" ] host [ ":" port ]
 userinfo      = *( unreserved / pct-encoded / sub-delims / ":" )
 host          = IP-literal / IPv4address / reg-name
 port          = *DIGIT

 IP-literal    = "[" ( IPv6address / IPvFuture  ) "]"
 IPv4address   = dec-octet "." dec-octet "." dec-octet "." dec-octet
 reg-name      = *( unreserved / pct-encoded / sub-delims )

 IPv6address   =                            6( h16 ":" ) ls32
			   /                       "::" 5( h16 ":" ) ls32
			   / [               h16 ] "::" 4( h16 ":" ) ls32
			   / [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
			   / [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
			   / [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32
			   / [ *4( h16 ":" ) h16 ] "::"              ls32
			   / [ *5( h16 ":" ) h16 ] "::"              h16
			   / [ *6( h16 ":" ) h16 ] "::"
 IPvFuture     = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )

 h16           = 1*4HEXDIG
 ls32          = ( h16 ":" h16 ) / IPv4address

 dec-octet     = DIGIT                 ; 0-9
			   / %x31-39 DIGIT         ; 10-99
			   / "1" 2DIGIT            ; 100-199
			   / "2" %x30-34 DIGIT     ; 200-249
			   / "25" %x30-35          ; 250-255

 path-abempty  = *( "/" segment )
 path-absolute = "/" [ segment-nz *( "/" segment ) ]
 path-rootless = segment-nz *( "/" segment )
 path-empty    = 0<pchar>

 segment       = *pchar
 segment-nz    = 1*pchar

 pchar         = unreserved / pct-encoded / sub-delims / ":" / "@"

 pct-encoded   = "%" HEXDIG HEXDIG
 unreserved    = ALPHA / DIGIT / "-" / "." / "_" / "~"
 sub-delims    = "!" / "$" / "&" / "'" / "(" / ")"
			   / "*" / "+" / "," / ";" / "="
*/
