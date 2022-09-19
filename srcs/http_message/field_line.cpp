#include "field_line.hpp"
#include "http_abnf_space.hpp"

static const std::string kCrLf = "\r\n";

FieldLine::FieldLine() : field_name_(), field_value_() {}

// obs-fold = OWS CRLF RWS
bool FieldLine::StartWithObsFold(const ThinString &str)
{
	std::size_t crlf_pos = str.find(kCrLf);
	if (crlf_pos == ThinString::npos) {
		return false;
	}
	ThinString ows = str.substr(0, crlf_pos);
	ThinString rws = str.substr(crlf_pos + kCrLf.size(), 1);
	return http_abnf::IsOws(ows) && http_abnf::IsRws(rws);
}
