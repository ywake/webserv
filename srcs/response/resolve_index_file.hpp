#ifndef RESOLVE_INDEX_FILE_HPP
#define RESOLVE_INDEX_FILE_HPP

#include <string>

#include "location_conf.hpp"
#include "result.hpp"

// Statクラスのエラーコードが返る
namespace response
{
	typedef std::string PartialPath;
	typedef std::string FullPath;

	result::Result<PartialPath> ResolveIndexFilePath(
		const FullPath                       &root,
		const PartialPath                    &request_path,
		const conf::LocationConf::IndexFiles &index_files
	);
} // namespace response

#endif
