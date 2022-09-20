#include "field_line.hpp"
#include "error.hpp"
#include "validate_field_line.hpp"

static const std::string kCrLf = "\r\n";

FieldLine::FieldLine() : field_name_(), field_value_() {}
FieldLine::FieldLine(const ThinString &line)
{
	std::size_t colon_pos = line.find(":");
	if (colon_pos == ThinString::npos) {
		throw Error("400");
	}
	ThinString name  = line.substr(0, colon_pos);
	ThinString value = line.substr(colon_pos + 1);
}

const ThinString &FieldLine::GetFieldName() const
{
	return field_value_;
}
const ThinString &FieldLine::GetFieldValue() const
{
	return field_name_;
}

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
