#include "string_array.hpp"

StringArray::StringArray() : strings_(), c_strings_() {}

StringArray::StringArray(const std::vector<std::string> &strings) : strings_(strings)
{
	InitCstrings(strings_);
}

StringArray::StringArray(const StringArray &other)
{
	*this = other;
}

StringArray &StringArray::operator=(const StringArray &rhs)
{
	if (&rhs == this) {
		return *this;
	}
	strings_ = rhs.strings_;
	InitCstrings(strings_);
	return *this;
}

const std::vector<std::string> &StringArray::Strings() const
{
	return strings_;
}

const char *const *StringArray::CArray() const
{
	return c_strings_.data();
}

void StringArray::InitCstrings(const std::vector<std::string> &strings)
{
	for (std::vector<std::string>::const_iterator it = strings.begin(); it != strings.end(); ++it) {
		c_strings_.push_back(it->c_str());
	}
	c_strings_.push_back(NULL);
}
