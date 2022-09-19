#ifndef FIELD_LINE_HPP
#define FIELD_LINE_HPP
#include <list>
#include <map>
#include <string>
#include <vector>

#include "basic_token.hpp"
#include "thin_string.hpp"

class FieldLines
{
  private:
	enum TokenId {
		kEmpty,
		kNormalTk,
		kCrLfTk,
		kObsFoldTk
	};

  private:
	typedef BasicToken<TokenId>     Token;
	typedef std::list<Token>        Tokens;
	typedef std::vector<ThinString> StringAry;

  public:
	FieldLines(const ThinString &str);

  private:
	Tokens    TokenizeLines(const ThinString &str);
	bool      IsValidTokenOrder(const Tokens &tokens);
	void      ReplaceObsFoldWithSpace(Tokens &tokens);
	StringAry ParseTokensToLines(Tokens &tokens);
	bool      IsObsFold(const ThinString &str);
	Token     CreateCrLfOrObsFoldToken(const ThinString &str);
	Token     CreateNormalToken(const ThinString &str);
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
