#include "field_line.hpp"
#include "error.hpp"
#include "http_abnf_space.hpp"

static const std::string kCrLf = "\r\n";

FieldLines::FieldLines(const ThinString &str)
{
	if (str.empty()) {
		return;
	}
	Tokens tokens = TokenizeLines(str);
	if (!IsValidTokenOrder(tokens)) {
		throw Error("400");
	}
	JoinTokensSeparatedByObsFold(tokens);
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
