#include "field_value.hpp"

HeaderValue::HeaderValue(const std::string &str) : value_(str) {}

HeaderValue::~HeaderValue() {}

// TODO
bool HeaderValue::operator==(const HeaderValue &rhs) const
{
	(void)rhs;
	return false;
}

bool HeaderValue::operator!=(const HeaderValue &rhs) const
{
	return !(*this == rhs);
}

std::ostream &operator<<(std::ostream &os, const HeaderValue &field_value)
{
	(void)field_value;
	return os;
}
