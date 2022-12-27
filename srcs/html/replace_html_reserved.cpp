#include <map>
#include <string>

namespace html
{
	typedef std::map<unsigned char, std::string> HtmlEscapes;

	static HtmlEscapes InitHtmlEscapes()
	{
		HtmlEscapes m;
		m['"'] = "&quot;";
		m['&'] = "&amp;";
		m['<'] = "&lt;";
		m['>'] = "&gt;";
		return m;
	}
	static const HtmlEscapes kHtmlEscapes = InitHtmlEscapes();

	std::string ReplaceHtmlReserved(const std::string &s)
	{
		std::string replaced;

		typedef std::map<unsigned char, std::string>::const_iterator MapItr;
		for (size_t i = 0; i < s.size(); i++) {
			unsigned char c  = s[i];
			MapItr        it = kHtmlEscapes.find(c);
			if (it == kHtmlEscapes.end()) {
				replaced += c;
			} else {
				replaced += kHtmlEscapes.at(c);
			}
		}
		return replaced;
	}
} // namespace html
