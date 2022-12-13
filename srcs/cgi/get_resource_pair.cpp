#include "get_resource_pair.hpp"

#include "webserv_utils.hpp"

namespace cgi
{
	Result<PathPair>
	GetResourcePair(const conf::LocationConf location_conf, const server::IRequest &request)
	{
		Path base_path = utils::JoinPath(location_conf.GetRoot(), request.Path());
		if (IsEndWithSlash(base_path)) {
			return FindFromIndexFiles(base_path, location_conf.GetIndexFiles());
		} else {
			return SplitPath(base_path);
		}
	}

	bool IsEndWithSlash(const std::string &str)
	{
		return !str.empty() && utils::GetLastChar(str) == '/';
	}

	Result<PathPair> FindFromIndexFiles(const Path &base_path, std::vector<Path> index_files)
	{
		std::vector<Path> candidates   = CombineIndexFiles(base_path, index_files);
		Result<Path>      correct_path = FindCorrectPath(candidates);
		return PathPair(correct_path.Val(), Path(""));
	}

	std::vector<Path> CombineIndexFiles(const Path &base_path, std::vector<Path> index_files)
	{
		std::vector<Path> combined_path_array;
		for (size_t i = 0; i < index_files.size(); ++i) {
			Path combined_path = utils::JoinPath(base_path, index_files[i]);
			combined_path_array.push_back(combined_path);
		}
		return combined_path_array;
	}

	Result<Path> FindCorrectPath(const std::vector<Path> &candidates)
	{
		for (size_t i = 0; i < candidates.size(); ++i) {
			Result<Path> cerrect_path = GetCorrectPath(candidates[i]);
			if (cerrect_path.IsOk()) {
				return cerrect_path;
			}
		}
		return Error("No correct path");
	}

	Result<Path> GetCorrectPath(const Path &path)
	{
		Result<Path> normalized_path = utils::NormalizePath(path);
		if (normalized_path.IsErr()) {
			return Error("Normalize path failed");
		}
		if (IsAccessiblePath(normalized_path.Val())) {
			return Error("Access failed");
		}
		return normalized_path;
	}

	bool IsAccessiblePath(Path path)
	{
		return access(path.c_str(), R_OK) == 0;
	}

	Result<PathPair> SplitPath(const Path &path)
	{
		std::vector<PathPair> candidates = CreatePathPairCandidates(path);
		return FindCorrectPair(candidates);
	}

	/**
	 * /example/index.html/path/info
	 *
	 * ↓candidates
	 *
	 * {{ "/example", "/index.html/path/info" }, { "/example/index.html" , "/path/info" },
	 * {"/example/index.html/path" , "/info" }, { "/example/index.html/path/info" , "" }}
	 */
	std::vector<PathPair> CreatePathPairCandidates(const CgiResponse::Path &path)
	{
		std::vector<PathPair> path_pair_array;
		std::vector<Path>     split = utils::Split(path, "/");
		for (std::vector<std::string>::iterator sep = split.begin(); sep < split.end(); ++sep) {
			if (sep->empty()) {
				continue;
			}
			Path script_path = utils::Join(std::vector<std::string>(split.begin(), sep + 1), "/");
			Path path_info   = utils::Join(std::vector<std::string>(sep + 1, split.end()), "/");
			if (!path_info.empty()) {
				path_info = "/" + path_info;
			}
			path_pair_array.push_back(PathPair(script_path, path_info));
		}
		return path_pair_array;
	}

	Result<PathPair> FindCorrectPair(const std::vector<PathPair> &candidates)
	{
		for (size_t i = 0; i < candidates.size(); ++i) {
			Result<Path> correct_path = GetCorrectPath(candidates[i].first);
			if (correct_path.IsOk()) {
				return PathPair(correct_path.Val(), candidates[i].second);
			}
		}
		return Error("No correct pair");
	}

} // namespace cgi
