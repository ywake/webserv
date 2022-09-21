#include "thin_string.hpp"
#include "validate_http_char.hpp"

static const std::string kWhiteSpaces = " \t";
static const std::string kCrLf        = "\r\n";

// TODO test
namespace http_abnf
{
	static bool IsWhiteSpaceOnly(const ThinString &str);

	// *( SP / HTAB )
	bool IsOws(const ThinString &str)
	{
		return str.empty() || IsWhiteSpaceOnly(str);
	}

	//  1*( SP / HTAB )
	bool IsRws(const ThinString &str)
	{
		return !str.empty() && IsWhiteSpaceOnly(str);
	}

	// obs-fold = OWS CRLF RWS
	bool StartWithObsFold(const ThinString &str)
	{
		std::size_t crlf_pos = str.find(kCrLf);
		if (crlf_pos == ThinString::npos) {
			return false;
		}
		ThinString ows = str.substr(0, crlf_pos);
		ThinString rws = str.substr(crlf_pos + kCrLf.size(), 1);
		return IsOws(ows) && IsRws(rws);
	}

	bool EndWithObsFold(const ThinString &str)
	{
		return str.TrimRight(kWhiteSpaces).EndWith(kCrLf);
	}

	static bool IsWhiteSpaceOnly(const ThinString &str)
	{
		for (std::size_t i = 0; i < str.size(); i++) {
			if (kWhiteSpaces.find(str.at(i)) == ThinString::npos) {
				return false;
			};
		}
		return true;
	}

	bool IsFieldName(const ThinString &str)
	{
		return IsToken(str);
	}

	/*
	  field-value   = *(field-content / obs-fold)

	  field-content = field-vchar [ 1*( SP / HTAB / field-vchar ) field-vchar ]
	  field-vchar   = VCHAR / obs-text
	  obs-text      = %x80-FF

	  obs-fold      = OWS CRLF RWS
	*/
	bool IsFieldValue(const ThinString &str)
	{
		return str.empty();
	}

} // namespace http_abnf
