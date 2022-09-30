#ifndef HEADER_SECTION_HPP
#define HEADER_SECTION_HPP
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "basic_token.hpp"
#include "field_line.hpp"
#include "header_value.hpp"
#include "thin_string.hpp"

class HeaderSection
{
  private:
	enum TokenId {
		kUndefined,
		kFieldLineTk,
		kCrLfTk
	};

  public:
	typedef std::list<HeaderValue>                               Values;
	typedef std::map<const std::string, std::list<HeaderValue> > Headers;

  private:
	typedef BasicToken<TokenId>     Token;
	typedef std::list<Token>        Tokens;
	typedef std::list<FieldLine>    Lines;
	typedef std::vector<ThinString> StringAry;

  private:
	Headers field_lines_;

  public:
	HeaderSection();
	HeaderSection(const ThinString &str);
	HeaderSection(const Headers &field_lines);

	bool    operator==(const HeaderSection &rhs) const;
	bool    operator!=(const HeaderSection &rhs) const;
	Values &operator[](const std::string &field_name);

	const Headers &GetMap() const;

	bool Contains(const std::string &field_name) const;

  private:
	Tokens    TokenizeLines(const ThinString &str) const;
	bool      IsValidTokenOrder(const Tokens &tokens) const;
	StringAry ParseTokensToLines(Tokens &tokens) const;
	Token     CreateFieldLineToken(const ThinString &str) const;
	Lines     ParseFieldLines(const Tokens &tokens) const;
	void      StoreFieldLines(const Lines &lines);
	Values    ParseEachHeader(const std::string &name, const ThinString &value);
};

std::ostream &operator<<(std::ostream &out, const HeaderSection &field_lines);

/*
  field-line    = field-name ":" OWS field-value OWS

  field-name    = token

  field-value   = *(field-content / obs-fold)
  field-content = field-vchar [ 1*( SP / HTAB / field-vchar ) field-vchar ]
  field-vchar   = VCHAR / obs-text
  obs-text      = %x80-FF

  obs-fold      = OWS CRLF RWS
  ; obs-foldのみの連続も可能で、SP一つと同等に扱う RFC2616 2.2

*/

#endif
