#include "field_line.hpp"
#include "http_exceptions.hpp"
#include "parse_http_utils.hpp"
#include "validate_field_line.hpp"

FieldLine::FieldLine() : field_name_(), field_value_() {}

FieldLine::FieldLine(const std::string &field_name, const std::string &field_value)
	: field_name_(field_name), field_value_(field_value)
{}

FieldLine::FieldLine(const ThinString &line)
{
	std::size_t colon_pos = line.find(":");
	if (colon_pos == ThinString::npos) {
		throw http::BadRequestException();
	}
	ThinString name  = line.substr(0, colon_pos);
	ThinString value = line.substr(colon_pos + 1);
	if (!http::abnf::IsFieldName(name)) {
		throw http::BadRequestException();
	}
	ThinString trimed_ows = http::TrimOws(value);
	if (!http::abnf::IsFieldValue(trimed_ows)) {
		throw http::BadRequestException();
	}
	field_name_  = name;
	field_value_ = trimed_ows;
}

const ThinString &FieldLine::GetFieldName() const
{
	return field_name_;
}

const ThinString &FieldLine::GetFieldValue() const
{
	return field_value_;
}

bool FieldLine::operator==(const FieldLine &rhs) const
{
	return field_name_ == rhs.GetFieldName() && field_value_ == rhs.GetFieldValue();
}

bool FieldLine::operator!=(const FieldLine &rhs) const
{
	return !(*this == rhs);
}

std::ostream &operator<<(std::ostream &os, const FieldLine &field_line)
{
	os << "\"" << field_line.GetFieldName() << "\":\"" << field_line.GetFieldValue() << "\"";
	return os;
}

// STR ":" OWS STR OBSFOLD STR OWS
// name = STR

// value =  OWS STR OBSFOLD STR OWS
// value =  OWS STR SP STR OWS
// value =  STR SP STR

// value = uniq_validate(STR SP STR)
