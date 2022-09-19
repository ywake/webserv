#include "field_liness.hpp"
#include "error.hpp"
#include "http_abnf_space.hpp"
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

FieldLiness::FieldLiness() {}

FieldLiness::FieldLiness(const ThinString &str)
{
	if (str.empty()) {
		return;
	}
	Tokens tokens = TokenizeLines(str);
	if (!IsValidTokenOrder(tokens)) {
		throw Error("400");
	}
	ParseFieldLines(tokens);
}

FieldLiness::Tokens FieldLiness::TokenizeLines(const ThinString &str) const
{
	Tokens tokens;

	for (ThinString remained = str; remained.size();) {
		Token token;
		if (remained.StartWith(kCrLf) && !StartWithObsFold(remained)) {
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
FieldLiness::Token FieldLiness::CreateFieldLineToken(const ThinString &str) const
{
	std::size_t token_len = 0;
	while (true) {
		token_len = str.FindAfter(kCrLf, token_len);
		if (token_len == std::string::npos) {
			token_len = str.size();
			break;
		}
		if (!StartWithObsFold(str.substr(token_len))) {
			break;
		}
		token_len += kCrLf.size();
	}
	return Token(str.substr(0, token_len), kFieldLineTk);
}

// obs-fold = OWS CRLF RWS
bool FieldLiness::StartWithObsFold(const ThinString &str) const
{
	std::size_t crlf_pos = str.find(kCrLf);
	if (crlf_pos == ThinString::npos) {
		return false;
	}
	ThinString ows = str.substr(0, crlf_pos);
	ThinString rws = str.substr(crlf_pos + kCrLf.size(), 1);
	return http_abnf::IsOws(ows) && http_abnf::IsRws(rws);
}

bool FieldLiness::IsValidTokenOrder(const Tokens &tokens) const
{
	if (tokens.empty()) {
		return true;
	}
	if (tokens.back().GetId() != kCrLfTk) {
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

void FieldLiness::ParseFieldLines(const Tokens &tokens)
{
	(void)tokens;
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

// std::string &FieldLiness::operator[](std::string field_name)
// {
// 	for (std::string::iterator it = field_name.begin(); it != field_name.end(); it++) {
// 		*it = std::tolower(*it);
// 	}
// 	return field_lines_[field_name];
// }
// void FieldLiness::ReplaceObsFoldWithSpace(Tokens &tokens) const
// {
// 	for (Tokens::iterator it = tokens.begin(); it != tokens.end(); it++) {
// 		if (it->GetId() == kObsFoldTk) {
// 			*it = Token(kSingleSpace, kFieldLineTk);
// 		}
// 	}
// }
