#ifndef FIELD_VALUE_HPP
#define FIELD_VALUE_HPP

#include <list>

#include "basic_token.hpp"
#include "thin_string.hpp"

class FieldValue
{
  private:
	ThinString field_value_;

  private:
	enum TokenId {
		kUndefined,
		kFieldContentTk,
		kObsFoldTk
	};

  private:
	typedef BasicToken<TokenId> Token;
	typedef std::list<Token>    Tokens;

  public:
	FieldValue();
	FieldValue(const ThinString &str);
	Tokens            Tokenize(const ThinString &str) const;
	FieldValue       &operator=(const FieldValue &other);
	const ThinString &GetValue() const;
};
/*
  field-value   = *(field-content / obs-fold)
  field-content = field-vchar [ 1*( SP / HTAB / field-vchar ) field-vchar ]
  field-vchar   = VCHAR / obs-text
  obs-text      = %x80-FF

  obs-fold      = OWS CRLF RWS
*/
#endif
