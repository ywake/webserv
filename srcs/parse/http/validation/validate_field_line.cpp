#include "validate_field_line.hpp"
#include "http_define.hpp"
#include "parse_abnf_core_rules.hpp"
#include "thin_string.hpp"
#include "validate_http_char.hpp"

static const std::size_t kObsFoldMinSize = 3;

namespace http
{
	namespace abnf
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

		bool IsRws(char c)
		{
			return kWhiteSpaces.find(c) != ThinString::npos;
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
			if (!str.EndWith(" ") && !str.EndWith("\t")) {
				return false;
			}
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
		*/
		bool IsFieldValue(const ThinString &str)
		{
			if (str.empty()) {
				return true;
			}
			const bool is_valid_start = IsFieldVchar(str.at(0)) || StartWithObsFold(str);
			const bool is_valid_end   = IsFieldVchar(str.at(str.size() - 1)) || EndWithObsFold(str);
			if (!is_valid_start || !is_valid_end) {
				return false;
			}
			for (std::size_t i = 0; i < str.size();) {
				if (IsFieldVchar(str.at(i)) || kWhiteSpaces.find(str.at(i)) != ThinString::npos) {
					i++;
				} else if (str.at(i) == '\r' && StartWithObsFold(str.substr(i, kObsFoldMinSize))) {
					i += kObsFoldMinSize;
				} else {
					return false;
				}
			}
			return true;
		}

		// field-vchar = VCHAR / obs-text
		bool IsFieldVchar(unsigned char c)
		{
			return ABNF::IsVchar(c) || IsObsText(c);
		}

		// obs-text = %x80-FF
		bool IsObsText(unsigned char c)
		{
			return 0x80 <= c;
		}

		static void TrimRightOws(std::string &s)
		{
			for (; !s.empty() && kWhiteSpaces.find(*s.rbegin()) != std::string::npos;) {
				s.erase(s.size() - 1);
			}
		}

		std::string TrimObsFold(const ThinString s)
		{
			std::string trimed;

			std::size_t i = 0;
			for (; i < s.size() && s.at(i) != ':'; i++) {
				trimed += s.at(i);
			}
			for (; i < s.size();) {
				if (StartWithObsFold(s.substr(i, kObsFoldMinSize))) {
					TrimRightOws(trimed);
					trimed += " ";
					i += kObsFoldMinSize;
					for (; i < s.size() && IsRws(s.at(i)); i++)
						;
				} else {
					trimed += s.at(i);
					i++;
				}
			}
			return trimed;
		}
	} // namespace abnf
} // namespace http
