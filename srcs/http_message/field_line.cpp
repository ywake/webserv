#include "field_line.hpp"
#include "error.hpp"
#include "http_abnf_space.hpp"
#include <list>

// STR CRLF STR OBSFOLD STR CRLF STR CRLF
//                                     ↑ 無かったらerror

// STR [CRLF] STR [OBSFOLD] STR [CRLF] STR [CRLF]

// STR [CRLF]
// STR [OBSFOLD] STR [CRLF]
// STR [CRLF]

// A : STR
// A : STR [OBSFOLD] STR
// A : STR

// A : STR
// A : STR SP STR
// A : STR

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
	ReplaceObsFoldWithSpace(tokens);
	StringAry lines = ParseTokensToLines(tokens);
	ParseFieldLines(lines);
}

FieldLiness::Tokens FieldLiness::TokenizeLines(const ThinString &str) const
{
	Tokens tokens;

	for (ThinString remained = str; remained.size();) {
		Token token;
		if (remained.substr(0, kCrLf.size()) == kCrLf &&
			!IsStartWithObsFold(str.substr(0, kCrLf.size() + 1))) {
			token = Token(kCrLf, kCrLfTk);
		} else {
			token = CreateNormalToken(remained);
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
FieldLiness::Token FieldLiness::CreateNormalToken(const ThinString &str) const
{
	std::size_t token_len = 0;
	while (true) {
		token_len = str.FindAfter(kCrLf, token_len);
		if (token_len == std::string::npos) {
			token_len = str.size();
			break;
		}
		if (!IsStartWithObsFold(str.substr(token_len))) {
			break;
		}
		token_len += kCrLf.size();
	}
	return Token(str.substr(0, token_len), kNormalTk);
}

FieldLiness::Token FieldLiness::CreateCrLfToken(const ThinString &str) const
{
	if (IsStartWithObsFold(str.substr(0, kCrLf.size() + 1))) {
		std::size_t ws_len = str.substr(kCrLf.size()).MeasureUntilNotOf(kWhiteSpaces);
		return Token(str.substr(0, kCrLf.size() + ws_len), kObsFoldTk);
	} else {
		return Token(kCrLf, kCrLfTk);
	}
}

// obs-fold = OWS CRLF RWS
bool FieldLiness::IsStartWithObsFold(const ThinString &str) const
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
		return false;
	}
	const bool is_start_with_obsfold = tokens.front().GetId() == kObsFoldTk;
	const bool is_end_with_crlf      = tokens.back().GetId() == kCrLfTk;
	if (is_start_with_obsfold || !is_end_with_crlf) {
		return false;
	}
	for (Tokens::const_iterator now = tokens.begin(), next = ++tokens.begin(); next != tokens.end();
		 now++, next++) {
		const bool has_empty_line = now->GetId() == kCrLfTk && next->GetId() != kNormalTk;
		const bool is_            = now->GetId() == kObsFoldTk && next->GetId() != kNormalTk;
		if (has_empty_line || is_) {
			return false;
		}
	}
	return true;
}

void FieldLiness::ParseFieldLines(const StringAry &lines)
{
	(void)lines;
}

void FieldLiness::ReplaceObsFoldWithSpace(Tokens &tokens) const
{
	for (Tokens::iterator it = tokens.begin(); it != tokens.end(); it++) {
		if (it->GetId() == kObsFoldTk) {
			*it = Token(kSingleSpace, kNormalTk);
		}
	}
}

FieldLiness::StringAry FieldLiness::ParseTokensToLines(Tokens &tokens) const
{
	StringAry lines;

	while (!tokens.empty()) {
		std::string line;
		while (!tokens.empty() && tokens.front().GetId() != kCrLfTk) {
			line += tokens.front().GetStr().ToString();
			tokens.pop_front();
		}
		if (tokens.front().GetId() == kCrLfTk) {
			tokens.pop_front();
		}
		lines.push_back(line);
	}
	return lines;
}
