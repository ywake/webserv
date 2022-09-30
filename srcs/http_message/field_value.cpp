#include "field_value.hpp"

FieldValue::FieldValueElem::FieldValueElem(const std::string &value) : value_(value) {}

FieldValue::FieldValueElem::~FieldValueElem() {}

FieldValue::FieldValue() {}

FieldValue::~FieldValue() {}

bool FieldValue::empty() const
{
	return values_.empty();
}

void FieldValue::push_back(const FieldLine &field_line)
{
	ThinString name = field_line.GetFieldName();
	if (name == "host") {
	} else if (name == "content-length") {
	} else if (name == "transfer-encoding") {
	} else if (name == "connection") {
	} else {
		values_.push_back(FieldValueElem(field_line.GetFieldValue().ToString()));
	}
}
// TODO
bool FieldValue::operator==(const FieldValue &rhs) const
{
	(void)rhs;
	return false;
}

bool FieldValue::operator!=(const FieldValue &rhs) const
{
	return !(*this == rhs);
}

std::ostream &operator<<(std::ostream &os, const FieldValue &field_value)
{
	(void)field_value;
	return os;
}
