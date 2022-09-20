#include "field_value.hpp"
#include "validate_field_line.hpp"

static const std::string kCrLf = "\r\n";

FieldValue::FieldValue() {}

// field-value   = *(field-content / obs-fold)
FieldValue::FieldValue(const ThinString &str) : field_value_()
{
	if (str.empty()) {
		return;
	}
	if (IsFieldVchar(str.at(0)) && !IsFieldVchar(str.at(str.size() - 1)) &&
		!StartWithObsFold(str) && !EndWithObsFold(str)) {
		throw Error(str);
	}
	Tokens tokens = Tokenize(str);
	for (itr; itr++) {
		if (!IsValidToken(itr)) {
			throw Error(str);
		}
	}
	fieldvalue_ = str;
}

FieldValue::Tokens FieldValue::Tokenize(const ThinString &str) const
{
	Tokens tokens;

	for (ThinString remained = str; remained.size();) {
		Token token;
		if (remained.StartWith(kCrLf)) {
			token = Token(remained.substr(0, kCrLf.size() + 1), kObsFoldTk);
		} else {
			token = CreateFieldContentToken(remained);
		}
		tokens.push_back(token);
		remained = remained.substr(token.GetLen());
	}
	return tokens;
}

FieldValue::Token FieldValue::CreateFieldContentToken(const ThinString &str) const
{
	std::size_t token_len = str.FindNotOf(kCrLf);
	if (token_len == ThinString::npos) {
		token_len = str.size();
	}
	return Token(str.substr(0, token_len), kFieldContentTk);
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
