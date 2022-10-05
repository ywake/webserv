#include "header_value.hpp"

HeaderValue::HeaderValue(const std::string &str) : value_(str) {}

HeaderValue::~HeaderValue() {}

bool HeaderValue::operator==(const HeaderValue &rhs) const
{
	return value_ == rhs.GetValue();
}

bool HeaderValue::operator!=(const HeaderValue &rhs) const
{
	return !(*this == rhs);
}

const std::string &HeaderValue::GetValue() const
{
	return value_;
}

std::ostream &operator<<(std::ostream &os, const HeaderValue &field_value)
{
	os << field_value.GetValue();
	return os;
}
