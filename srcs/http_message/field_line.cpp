#include "field_line.hpp"
#include "error.hpp"
#include "field_value.hpp"
#include "validate_field_line.hpp"

static const std::string kCrLf        = "\r\n";
static const std::string kWhiteSpaces = " \t";

FieldLine::FieldLine() : field_name_(), field_value_() {}

// TODO test
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
	ThinString trimed_ows = TrimOws(value);
	if (http_abnf::IsFieldValue(trimed_ows)) {
		throw Error("400");
	}
	field_name_  = name;
	field_value_ = trimed_ows;
}

ThinString FieldLine::TrimOws(const ThinString &value)
{
	ThinString  trimed_left = value.TrimLeft(kWhiteSpaces);
	std::size_t end_idx     = trimed_left.RFindNotOf(kWhiteSpaces);
	if (end_idx == ThinString::npos) {
		return "";
	}
	ThinString trimed_ows          = trimed_left.substr(0, end_idx + 1);
	ThinString trimed_with_obsfold = trimed_left.substr(0, end_idx + 2);
	if (http_abnf::EndWithObsFold(trimed_with_obsfold)) {
		return trimed_with_obsfold;
	} else {
		return trimed_ows;
	}
}

const ThinString &FieldLine::GetFieldName() const
{
	return field_value_;
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
