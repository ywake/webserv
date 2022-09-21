#ifndef FIELD_LINES_HPP
#define FIELD_LINES_HPP
#include <list>
#include <map>
#include <string>
#include <vector>

#include "basic_token.hpp"
#include "field_line.hpp"
#include "thin_string.hpp"

class FieldLines
{
  public:
	enum TokenId {
		kUndefined,
		kFieldLineTk,
		kCrLfTk,
		kObsFoldTk // TODO後で消す
	};

  public:
	typedef std::list<std::string> Values;

  public:
	typedef BasicToken<TokenId>     Token;
	typedef std::list<Token>        Tokens;
	typedef std::list<FieldLine>    Lines;
	typedef std::vector<ThinString> StringAry;

  private:
	std::map<const std::string, Values> field_lines_;

  public:
	FieldLines();
	FieldLines(const ThinString &str);
	Tokens TokenizeLines(const ThinString &str) const;

	bool    operator==(const FieldLines &rhs) const;
	bool    operator!=(const FieldLines &rhs) const;
	Values &operator[](std::string field_name);

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
