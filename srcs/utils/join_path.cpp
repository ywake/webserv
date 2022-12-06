#include <string>

namespace utils
{
	std::string JoinPath(const std::string &parent, const std::string &child)
	{
		if (parent.empty() || child.empty()) {
			return parent.empty() ? child : parent;
		}
		char parent_back = parent[parent.size() - 1];
		char child_front = child[0];

		if (parent_back != '/' && child_front != '/') {
			return parent + '/' + child;
		} else if (parent_back == '/' && child_front == '/') {
			return parent.substr(0, parent.size() - 1) + child;
		} else {
			return parent + child;
		}
	}
} // namespace utils
