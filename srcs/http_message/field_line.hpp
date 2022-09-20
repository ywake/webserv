#ifndef FIELD_LINE_HPP
#define FIELD_LINE_HPP

#include "field_value.hpp"
#include "thin_string.hpp"

class FieldLine
{
  private:
	ThinString field_name_;
	FieldValue field_value_;

  public:
	FieldLine();
	FieldLine(const ThinString &line);
	const ThinString &GetFieldName() const;
	const ThinString &GetFieldValue() const;
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
