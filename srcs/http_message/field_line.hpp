#ifndef FIELD_LINE_HPP
#define FIELD_LINE_HPP
#include <map>
#include <string>
#include <vector>

#include "thin_string.hpp"

class FieldLines
{
  private:
	class Token
	{
	  public:
		enum TokenId {
			kEmpty,
			kNormalTk,
			kCrLfTk,
			kObsFoldTk
		};

	  private:
		ThinString s_;
		TokenId    id_;

	  public:
		Token();
		Token(const ThinString &str, TokenId Id);
		TokenId           GetId() const;
		const ThinString &GetStr() const;
		std::size_t       GetLen() const;
	};

  private:
	typedef std::vector<Token>      Tokens;
	typedef std::vector<ThinString> StringAry;

  public:
	FieldLines(const ThinString &str);

  private:
	Tokens TokenizeLines(const ThinString &str);
	bool   IsObsFold(const ThinString &str);
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
