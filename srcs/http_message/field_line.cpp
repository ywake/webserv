#include "field_line.hpp"
#include "error.hpp"
#include "http_abnf_space.hpp"
#include <list>

// TODO ThinString
static const std::string kCrLf        = "\r\n";
static const std::string kWhiteSpaces = " \t";
static const std::string kSingleSpace = " ";

FieldLines::FieldLines(const ThinString &str)
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

FieldLines::Tokens FieldLines::TokenizeLines(const ThinString &str)
{
	Tokens tokens;

	for (ThinString remained = str; remained.size();) {
		Token token;
		if (remained.substr(0, kCrLf.size()) == kCrLf) {
			token = CreateCrLfOrObsFoldToken(remained);
		} else {
			token = CreateNormalToken(remained);
		}
		tokens.push_back(token);
		remained = remained.substr(token.GetLen());
	}
	return tokens;
}

FieldLines::Token FieldLines::CreateNormalToken(const ThinString &str)
{
	std::size_t token_len = str.MeasureUntil(kCrLf);
	return Token(str.substr(0, token_len), kNormalTk);
}

FieldLines::Token FieldLines::CreateCrLfOrObsFoldToken(const ThinString &str)
{
	if (IsObsFold(str.substr(0, kCrLf.size() + 1))) {
		std::size_t ws_len = str.substr(kCrLf.size()).MeasureUntilNotOf(kWhiteSpaces);
		return Token(str.substr(0, kCrLf.size() + ws_len), kObsFoldTk);
	} else {
		return Token(kCrLf, kCrLfTk);
	}
}

//                       ↓ 無かったらerror
// STR CRLF STR OBSFOLD STR CRLF STR CRLF
// ↓　　　　　　　　　　　　　　　　　　↑ 無かったらerror
// STR CRLF STR +  SP + STR CRLF STR CRLF
// ↓
// STR STR STR

// obs-fold = OWS CRLF RWS
bool FieldLines::IsObsFold(const ThinString &str)
{
	std::size_t crlf_pos = str.find(kCrLf);
	if (crlf_pos == ThinString::npos) {
		return false;
	}
	ThinString ows = str.substr(0, crlf_pos);
	ThinString rws = str.substr(crlf_pos + kCrLf.size());
	return http_abnf::IsOws(ows) && http_abnf::IsRws(rws);
}

bool FieldLines::IsValidTokenOrder(const Tokens &tokens)
{
	(void)tokens;
	return false;
}

void FieldLines::ParseFieldLines(const StringAry &lines)
{
	(void)lines;
}

void FieldLines::ReplaceObsFoldWithSpace(Tokens &tokens)
{
	for (Tokens::iterator it = tokens.begin(); it != tokens.end(); it++) {
		if (it->GetId() == kObsFoldTk) {
			*it = Token(kSingleSpace, kNormalTk);
		}
	}
}

FieldLines::StringAry FieldLines::ParseTokensToLines(Tokens &tokens)
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

FieldLines::Token::Token() : s_(), id_() {}
FieldLines::Token::Token(const ThinString &str, TokenId id) : s_(str), id_(id) {}

FieldLines::Token::TokenId FieldLines::Token::GetId() const
{
	return id_;
}

const ThinString &FieldLines::Token::GetStr() const
{
	return s_;
}

std::size_t FieldLines::Token::GetLen() const
{
	return s_.size();
}
