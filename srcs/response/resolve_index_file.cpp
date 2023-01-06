#include "resolve_index_file.hpp"
#include "debug.hpp"
#include "stat.hpp"
#include "webserv_utils.hpp"

namespace response
{
	typedef conf::LocationConf::IndexFiles IndexFiles;

	result::ErrCode kForbidden = new Error("not readable");

	static Result<PartialPath> FindReadableIndexFile(
		const FullPath &root, const PartialPath &base_path, const IndexFiles &index_files
	);

	result::Result<PartialPath> ResolveIndexFilePath(
		const FullPath &root, const PartialPath &request_path, const IndexFiles &index_files
	)
	{
		result::Result<Stat> st = Stat::FromPath(utils::JoinPath(root, request_path));
		if (st.IsErr()) {
			return st.Err();
		}
		PartialPath path = request_path;
		while (utils::EndWith(path, "/")) {
			Result<PartialPath> index_file_path = FindReadableIndexFile(root, path, index_files);
			if (index_file_path.IsErr()) {
				break;
			}
			path = index_file_path.Val();
			log("found index file", path);
		}
		log("index file result", path);
		if (utils::IsReadablePath(utils::JoinPath(root, path))) {
			return path;
		} else {
			return kForbidden;
		}
	}

	static Result<PartialPath> FindReadableIndexFile(
		const FullPath &root, const PartialPath &base_path, const IndexFiles &index_files
	)
	{
		for (IndexFiles::const_iterator it = index_files.begin(); it != index_files.end(); ++it) {
			const PartialPath index_path = utils::JoinPath(base_path, *it);
			const FullPath    abs_path   = utils::JoinPath(root, index_path);
			if (Stat::FromPath(abs_path).IsErr() || !utils::IsReadablePath(abs_path)) {
				continue;
			}
			Result<PartialPath> normalized = utils::NormalizePath(index_path);
			if (normalized.IsErr()) {
				continue;
			}
			return normalized.Val();
		}
		return Error();
	}
} // namespace response
