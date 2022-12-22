#include "html_utils.hpp"
#include "percent_encode.hpp"
#include "webserv_utils.hpp"

namespace html
{
	std::string CreateHyperLink(const std::string &path, const std::string &name)
	{
		std::string link_path = uri::PercentEncode(path);
		std::string link_name = ReplaceHtmlReserved(name);
		std::string link      = "<a href=\"" + link_path + "\">" + link_name + "</a>";
		return link;
	}

} // namespace html
