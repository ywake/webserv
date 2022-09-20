#include "field_line.hpp"
#include "error.hpp"
#include "field_value.hpp"
#include "validate_field_line.hpp"

FieldLine::FieldLine() : field_name_(), field_value_() {}

FieldLine::FieldLine(const ThinString &line)
{
	std::size_t colon_pos = line.find(":");
	if (colon_pos == ThinString::npos) {
		throw Error("400");
	}
	ThinString name  = line.substr(0, colon_pos);
	ThinString value = line.substr(colon_pos + 1);
	if (!http_abnf::IsFieldName(name)) {
		throw Error("400");
	}
	field_name_  = name;
	field_value_ = FieldValue(value);
}

const ThinString &FieldLine::GetFieldName() const
{
	return field_value_.GetValue();
}

const ThinString &FieldLine::GetFieldValue() const
{
	return field_name_;
}

// STR ":" OWS STR OBSFOLD STR OWS
// name = STR

// value =  OWS STR OBSFOLD STR OWS
// value =  OWS STR SP STR OWS
// value =  STR SP STR

// value = uniq_validate(STR SP STR)
