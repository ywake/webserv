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
	return Token(str.substr(0, token_len), Token::kNormalTk);
}

FieldLines::Token FieldLines::CreateCrLfOrObsFoldToken(const ThinString &str)
{
	if (IsObsFold(str.substr(0, kCrLf.size() + 1))) {
		std::size_t ws_len = str.substr(kCrLf.size()).MeasureUntilNotOf(kWhiteSpaces);
		return Token(str.substr(0, kCrLf.size() + ws_len), Token::kObsFoldTk);
	} else {
		return Token(kCrLf, Token::kCrLfTk);
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

void FieldLines::ReplaceObsFoldWithSpace(Tokens &tokens)
{
	for (Tokens::iterator it = tokens.begin(); it != tokens.end(); it++) {
		if (it->GetId() == Token::kObsFoldTk) {
			*it = Token(kSingleSpace, Token::kNormalTk);
		}
	}
}

FieldLines::StringAry FieldLines::ParseTokensToLines(Tokens &tokens)
{
	StringAry lines;

	while (!tokens.empty()) {
		std::string line;
		while (!tokens.empty() && tokens.front().GetId() != Token::kCrLfTk) {
			line += tokens.front().GetStr().ToString();
			tokens.pop_front();
		}
		if (tokens.front().GetId() == Token::kCrLfTk) {
			tokens.pop_front();
		}
		lines.push_back(line);
	}
	return lines;
}
