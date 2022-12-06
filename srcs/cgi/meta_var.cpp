#include "meta_var.hpp"

namespace cgi
{

	/**
	 * Constructor/destructor
	 */

	MetaVar::MetaVar() : name_(), value_(), is_case_sensitive_(false) {}

	MetaVar::MetaVar(const MetaVar &other)
		: name_(other.name_), value_(other.value_), is_case_sensitive_(other.is_case_sensitive_)
	{}

	MetaVar::MetaVar(const std::string &name, const std::string &value, bool is_case_sensitive)
		: name_(name), value_(value), is_case_sensitive_(is_case_sensitive)
	{}

	MetaVar::~MetaVar() {}

	/**
	 * Getters
	 */
	const std::string &MetaVar::Name() const
	{
		return name_;
	}

	const std::string &MetaVar::Value() const
	{
		return value_;
	}

	bool MetaVar::IsCaseSensitive() const
	{
		return is_case_sensitive_;
	}

	/**
	 * Operator overloads
	 */
	MetaVar &MetaVar::operator=(const MetaVar &rhs)
	{
		if (this == &rhs) {
			return *this;
		}
		name_              = rhs.name_;
		value_             = rhs.value_;
		is_case_sensitive_ = rhs.is_case_sensitive_;
		return *this;
	}

} // namespace cgi
