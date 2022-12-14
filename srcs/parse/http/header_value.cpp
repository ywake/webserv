#include "header_value.hpp"

HeaderValue::HeaderValue(const std::string &str) : value_(str) {}

HeaderValue::HeaderValue(const HeaderValue &other)
{
	*this = other;
}

HeaderValue::~HeaderValue() {}

HeaderValue &HeaderValue::operator=(const HeaderValue &rhs)
{
	if (this == &rhs) {
		return *this;
	}
	value_ = rhs.value_;
	return *this;
}

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

bool HeaderValue::empty() const
{
	return value_.empty();
}

std::ostream &operator<<(std::ostream &os, const HeaderValue &field_value)
{
	os << field_value.GetValue();
	return os;
}
