#ifndef FIELD_LINE_HPP
#define FIELD_LINE_HPP
#include <list>
#include <map>
#include <string>
#include <vector>

#include "basic_token.hpp"
#include "thin_string.hpp"

class FieldLiness
{
  public:
	enum TokenId {
		kEmpty,
		kNormalTk,
		kCrLfTk,
		kObsFoldTk
	};

  public:
	typedef BasicToken<TokenId>     Token;
	typedef std::list<Token>        Tokens;
	typedef std::vector<ThinString> StringAry;

  public:
	FieldLiness();
	FieldLiness(const ThinString &str);
	Tokens TokenizeLines(const ThinString &str) const;

  private:
	bool      IsValidTokenOrder(const Tokens &tokens) const;
	void      ReplaceObsFoldWithSpace(Tokens &tokens) const;
	StringAry ParseTokensToLines(Tokens &tokens) const;
	bool      IsStartWithObsFold(const ThinString &str) const;
	Token     CreateCrLfToken(const ThinString &str) const;
	Token     CreateNormalToken(const ThinString &str) const;
	void      ParseFieldLines(const StringAry &lines);
};
// TODO BNFミスってる
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
