#include "header_section.hpp"
#include "field_line.hpp"
#include "http_exceptions.hpp"
#include "validate_field_line.hpp"
#include "webserv_utils.hpp"

#include <list>

// [KEY:VAL] [CRLF] [KEY:VAL OBSFOLD VAL] [CRLF] [KEY:VAL] [CRLF]

// KEY : VAL
// KEY : VAL [OBSFOLD] VAL
// KEY : VAL

// KEY : VAL
// KEY : VAL SP VAL
// KEY : VAL

// TODO ThinString
static const std::string kCrLf        = "\r\n";
static const std::string kWhiteSpaces = " \t";
static const std::string kSingleSpace = " ";

HeaderSection::HeaderSection() {}

HeaderSection::HeaderSection(const ThinString &str)
{
	if (str.empty()) {
		return;
	}
	Tokens tokens = TokenizeLines(str);
	if (!IsValidTokenOrder(tokens)) {
		throw BadRequestException();
	}
	Lines lines = ParseFieldLines(tokens);
	StoreFieldLines(lines);
}

HeaderSection::HeaderSection(const std::map<const std::string, Values> &field_lines)
	: field_lines_(field_lines)
{}

HeaderSection::Tokens HeaderSection::TokenizeLines(const ThinString &str) const
{
	Tokens tokens;

	for (ThinString remained = str; remained.size();) {
		Token token;
		if (remained.StartWith(kCrLf) && !http_abnf::StartWithObsFold(remained)) {
			token = Token(kCrLf, kCrLfTk);
		} else {
			token = CreateFieldLineToken(remained);
		}
		tokens.push_back(token);
		remained = remained.substr(token.GetLen());
	}
	return tokens;
}

// [message1: Hello, World!]
// ↓
// [message2: Hello
//	World!]
// ↓
// [message3: Hello, World!]
HeaderSection::Token HeaderSection::CreateFieldLineToken(const ThinString &str) const
{
	std::size_t token_len = 0;
	while (true) {
		token_len = str.FindAfter(kCrLf, token_len);
		if (token_len == std::string::npos) {
			token_len = str.size();
			break;
		}
		if (!http_abnf::StartWithObsFold(str.substr(token_len))) {
			break;
		}
		token_len += kCrLf.size();
	}
	return Token(str.substr(0, token_len), kFieldLineTk);
}

bool HeaderSection::IsValidTokenOrder(const Tokens &tokens) const
{
	if (tokens.empty()) {
		return true;
	}
	if (tokens.front().GetId() == kCrLfTk || tokens.back().GetId() != kCrLfTk) {
		return false;
	}
	for (Tokens::const_iterator now = tokens.begin(), next = ++tokens.begin(); next != tokens.end();
		 now++, next++) {
		const bool has_empty_line = now->GetId() == kCrLfTk && next->GetId() == kCrLfTk;
		if (has_empty_line) {
			return false;
		}
	}
	return true;
}

HeaderSection::Lines HeaderSection::ParseFieldLines(const Tokens &tokens) const
{
	Lines lines;

	for (Tokens::const_iterator it = tokens.begin(); it != tokens.end(); it++) {
		if (it->GetId() == kCrLfTk) {
			continue;
		}
		lines.push_back(FieldLine(it->GetStr()));
	}
	return lines;
}

/*
TODO どっちでパースするか
[1]	value validation
	name結合可能？
	field_lines_[name] += ", " + value
	データ構造はmap<name, std::set>
	TRはcase insensitive
*/
void HeaderSection::StoreFieldLines(const Lines &lines)
{
	for (Lines::const_iterator it = lines.begin(); it != lines.end(); it++) {
		const std::string name  = utils::ToLowerString(it->GetFieldName().ToString());
		const std::string value = it->GetFieldValue().ToString();
		if (field_lines_[name].empty()) {
			field_lines_[name] = value;
		} else {
			field_lines_[name] += ", " + value;
		}
	}
}

std::string &HeaderSection::operator[](const std::string &field_name)
{
	return field_lines_[utils::ToLowerString(field_name)];
}

bool HeaderSection::operator==(const HeaderSection &rhs) const
{
	return field_lines_ == rhs.field_lines_;
}

bool HeaderSection::operator!=(const HeaderSection &rhs) const
{
	return !(*this == rhs);
}
// FieldLiness::Token FieldLiness::CreateCrLfToken(const ThinString &str) const
// {
// 	if (StartWithObsFold(str.substr(0, kCrLf.size() + 1))) {
// 		std::size_t ws_len = str.substr(kCrLf.size()).MeasureUntilNotOf(kWhiteSpaces);
// 		return Token(str.substr(0, kCrLf.size() + ws_len), kObsFoldTk);
// 	} else {
// 		return Token(kCrLf, kCrLfTk);
// 	}
// }

// void FieldLiness::ReplaceObsFoldWithSpace(Tokens &tokens) const
// {
// 	for (Tokens::iterator it = tokens.begin(); it != tokens.end(); it++) {
// 		if (it->GetId() == kObsFoldTk) {
// 			*it = Token(kSingleSpace, kFieldLineTk);
// 		}
// 	}
// }

const HeaderSection::Headers &HeaderSection::GetMap() const
{
	return field_lines_;
}

bool HeaderSection::Contains(const std::string &field_name) const
{
	return field_lines_.find(utils::ToLowerString(field_name)) != field_lines_.end();
}

std::ostream &operator<<(std::ostream &os, const HeaderSection &field_lines)
{
	HeaderSection::Headers headers = field_lines.GetMap();
	for (HeaderSection::Headers::const_iterator it = headers.begin(); it != headers.end(); it++) {
		const std::string name   = it->first;
		const std::string values = it->second;
		os << name << ": " << values << "\n";
	}
	return os;
}
