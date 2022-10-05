#ifndef FIELD_LINE_HPP
#define FIELD_LINE_HPP

#include "thin_string.hpp"
#include <iostream>

class FieldLine
{
  private:
	ThinString field_name_;
	ThinString field_value_;

  public:
	FieldLine();
	FieldLine(const std::string &field_name, const std::string &field_value);
	FieldLine(const ThinString &line);
	const ThinString &GetFieldName() const;
	const ThinString &GetFieldValue() const;

	bool operator==(const FieldLine &rhs) const;
	bool operator!=(const FieldLine &rhs) const;
};

std::ostream &operator<<(std::ostream &os, const FieldLine &field_line);

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
