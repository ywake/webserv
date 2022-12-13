#include "cgi_response.hpp"

#ifndef GET_RESOURCE_PAIR_HPP
#define GET_RESOURCE_PAIR_HPP

namespace cgi
{
	typedef CgiResponse::Path     Path;
	typedef CgiResponse::PathPair PathPair;

	Result<PathPair>
		 GetResourcePair(const conf::LocationConf location_conf, const server::IRequest &request);
	bool IsEndWithSlash(const std::string &str);
	Result<PathPair>      FindFromIndexFiles(const Path &base_path, std::vector<Path> index_files);
	std::vector<Path>     CombineIndexFiles(const Path &base_path, std::vector<Path> index_files);
	Result<Path>          FindCorrectPath(const std::vector<Path> &candidates);
	Result<Path>          GetCorrectPath(const Path &path);
	bool                  IsAccessiblePath(Path path);
	Result<PathPair>      SplitPath(const Path &path);
	std::vector<PathPair> CreatePathPairCandidates(const CgiResponse::Path &path);
	Result<PathPair>      FindCorrectPair(const std::vector<PathPair> &candidates);

} // namespace cgi
#endif
