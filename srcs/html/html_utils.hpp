#ifndef HTML_UTILS_HPP
#define HTML_UTILS_HPP

#include <string>

namespace html
{
	std::string ReplaceHtmlReserved(const std::string &s);
	std::string CreateHyperLink(const std::string &path, const std::string &name);

} // namespace html

#endif
