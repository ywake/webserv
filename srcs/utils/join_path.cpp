#include <string>

namespace utils
{
	std::string JoinPath(const std::string &parent, const std::string &child)
	{
		if (parent.empty() || child.empty()) {
			return parent.empty() ? child : parent;
		}
		if (*parent.rbegin() != '/' && *child.begin() != '/') {
			return parent + '/' + child;
		} else if (*parent.rbegin() == '/' && *child.begin() == '/') {
			return parent.substr(0, parent.size() - 1) + child;
		} else {
			return parent + child;
		}
	}
} // namespace utils
