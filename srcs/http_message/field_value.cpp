#include "field_value.hpp"
#include "validate_field_line.hpp"

FieldValue::FieldValue() {}

// field-value   = *(field-content / obs-fold)
FieldValue::FieldValue(const ThinString &str) : field_value_()
{
	if (str.empty()) {
		return;
	}
	Tokens tokens = Tokenize(str);
	for (itr; itr++) {
		if (!IsValidToken()) {
			throw Error();
		}
	}
	fieldvalue_ = str;
}

FieldValue::Tokens FieldValue::Tokenize(const ThinString &str) const
{
	Tokens tokens;

	for (ThinString remained = str; remained.size();) {
		Token token;
		if (http_abnf::StartWithObsFold(remained)) {
			token = Token(kCrLf, kCrLfTk);
		} else {
			token = CreateFieldLineToken(remained);
		}
		tokens.push_back(token);
		remained = remained.substr(token.GetLen());
	}
	return tokens;
}

const ThinString &FieldValue::GetValue() const
{
	return field_value_;
}

FieldValue &FieldValue::operator=(const FieldValue &other)
{
	if (this == &other) {
		return *this;
	}
	field_value_ = other.field_value_;
}
