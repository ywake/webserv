#include <deque>
#include <vector>

#include "thin_string.hpp"
#include "webserv_utils.hpp"

namespace utils
{
	typedef std::vector<ThinString> Strings;

	static bool IsDotSegment(const ThinString &path_segment)
	{
		return path_segment == "." || path_segment == "..";
	}

	static Strings RemoveDotSegments(const Strings &segments)
	{
		Strings dot_removed;

		for (Strings::const_iterator it = segments.begin(); it != segments.end(); ++it) {
			const ThinString &segment = *it;
			if (!IsDotSegment(segment)) {
				dot_removed.push_back(segment);
			} else if (segment == ".." && !dot_removed.empty()) {
				dot_removed.pop_back();
			}
		}
		return dot_removed;
	}

	static std::string JoinPathSegments(const Strings &segments)
	{
		if (segments.empty()) {
			return "";
		}
		std::string path;
		for (Strings::const_iterator it = segments.begin(); it != segments.end(); ++it) {
			path += it->ToString() + "/";
		}
		return path.erase(path.size() - 1);
	}

	ThinString NromalizePath(const ThinString &path)
	{
		const Strings segments    = Split(path, "/");
		Strings       dot_removed = RemoveDotSegments(TrimEmpty(segments));
		std::string   normalized  = JoinPathSegments(dot_removed);
		if (normalized.empty()) { // including path.empty() && segments.empty()
			return "/";
		}
		if (path.back() == '/' || IsDotSegment(segments.back())) {
			normalized += "/";
		}
		return "/" + normalized;
	}
} // namespace utils
