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

	static Result<Strings> RemoveDotSegments(const Strings &segments)
	{
		Strings dot_removed;

		for (Strings::const_iterator it = segments.begin(); it != segments.end(); ++it) {
			const ThinString &segment = *it;
			if (!IsDotSegment(segment)) {
				dot_removed.push_back(segment);
				continue;
			}
			if (segment == "..") {
				if (dot_removed.empty()) {
					return Error("Invalid path");
				}
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

	Result<std::string> NormalizePath(const ThinString &path)
	{
		bool            is_absolute = *path.begin() == '/';
		const Strings   segments    = TrimEmpty(Split(path, "/"));
		Result<Strings> dot_removed = RemoveDotSegments(segments);
		if (dot_removed.IsErr()) {
			return dot_removed.Err();
		}
		std::string normalized = JoinPathSegments(dot_removed.Val());

		if (normalized.empty()) { // including path.empty() && segments.empty()
			return std::string(is_absolute ? "/" : "");
		}
		if (path.back() == '/' || IsDotSegment(segments.back())) {
			normalized += "/";
		}
		return (is_absolute ? "/" : "") + normalized;
	}

	Result<std::string> HttpNormalizePath(const ThinString &path)
	{
		Result<std::string> res = NormalizePath(ThinString(path));
		if (res.IsErr()) {
			return res.Err();
		}
		std::string &normalized = res.Val();
		if (normalized.empty()) {
			return std::string("/");
		}
		if (*normalized.begin() != '/') {
			return "/" + normalized;
		}
		return normalized;
	}
} // namespace utils
