#ifndef FIELD_LINESS_HPP
#define FIELD_LINESS_HPP
#include <list>
#include <map>
#include <string>
#include <vector>

#include "basic_token.hpp"
#include "field_line.hpp"
#include "thin_string.hpp"

class FieldLiness
{
  private:
	std::map<const std::string, std::string> field_lines_;

  public:
	enum TokenId {
		kEmpty,
		kFieldLineTk,
		kCrLfTk,
		kObsFoldTk // TODO後で消す
	};

  public:
	typedef BasicToken<TokenId>     Token;
	typedef std::list<Token>        Tokens;
	typedef std::list<FieldLine>    Lines;
	typedef std::vector<ThinString> StringAry;

  public:
	FieldLiness();
	FieldLiness(const ThinString &str);
	Tokens TokenizeLines(const ThinString &str) const;

	std::string &operator[](std::string field_name);

  private:
	bool      IsValidTokenOrder(const Tokens &tokens) const;
	StringAry ParseTokensToLines(Tokens &tokens) const;
	Token     CreateFieldLineToken(const ThinString &str) const;
	Lines     ParseFieldLines(const Tokens &tokens) const;
	void      StoreFieldLines(const Lines &lines);
};

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
