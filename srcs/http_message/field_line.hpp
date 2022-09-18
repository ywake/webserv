#ifndef FIELD_LINE_HPP
#define FIELD_LINE_HPP
#include <list>
#include <map>
#include <string>
#include <vector>

#include "thin_string.hpp"

class FieldLines
{
  private:
	// TODO　外に出す
	class Token
	{
	  public:
		enum TokenId {
			kEmpty,
			kNormalTk,
			kCrLfTk,
			kObsFoldTk
		};
	// TODO　外に出す
	class Token
	{
	  private:
		ThinString s_;
		TokenId    id_;

	  public:
		Token();
		Token(const ThinString &str, TokenId id);
		TokenId           GetId() const;
		const ThinString &GetStr() const;
		std::size_t       GetLen() const;
	};

  private:
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

/*
  field-lines   = *( one-line )
  one-line      = *(field-line obs-fold ) field-line CRLF
  field-line    = field-name ":" OWS field-value OWS

  field-name    = token

  field-value   = *field-content
  field-content = field-vchar [ 1*( SP / HTAB / field-vchar ) field-vchar ]
  field-vchar   = VCHAR / obs-text
  obs-text      = %x80-FF
  obs-fold      = OWS CRLF RWS

*/

#endif
