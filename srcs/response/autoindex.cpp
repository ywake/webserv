#include <cerrno>
#include <cstring>
#include <ctime>
#include <dirent.h>
#include <map>
#include <set>
#include <string>
#include <sys/stat.h>
#include <vector>

#include "debug.hpp"
#include "html_utils.hpp"
#include "libc_err.hpp"
#include "result.hpp"
#include "webserv_utils.hpp"

namespace autoindex
{
	typedef std::set<std::string>              SortedNames;
	typedef std::map<std::string, struct stat> EntryStats;
	typedef std::vector<std::string>           EntryNames;

	typedef struct CategorizedEntry {
		EntryNames dirs;
		EntryNames others;
	} CategorizedEntry;

	static const std::size_t kMaxNameWidth     = 50;
	static const std::size_t kMaxFileSizeWidth = 20;
	static const std::string kEllipsis         = "..>";

	static std::string CreateHyperLink(const std::string &path)
	{
		std::string link_name = utils::OmitExcessBytes(path, kMaxNameWidth, kEllipsis);
		return html::CreateHyperLink(path, link_name);
	}

	static std::string CreateSpacePadding(const std::string &s, std::size_t max_width)
	{
		if (s.size() >= max_width) {
			return "";
		}
		return std::string(max_width - s.size(), ' ');
	}

	// "18-Dec-2022 03:36"
	static std::string CreateTimeStamp(const struct timespec &ts)
	{
		struct tm tm = {};
		char      buf[1000];

		gmtime_r(&ts.tv_sec, &tm); // タイムスタンプバグってもいいしシカトする
		strftime(buf, sizeof(buf), "%d-%b-%Y %H:%M", &tm);
		return std::string(buf);
	}

	static std::string CreateDirIndex(const EntryNames &entry_names, const EntryStats &entry_stats)
	{
		std::string index = CreateHyperLink("../") + "\n";

		for (EntryNames::const_iterator it = entry_names.begin(); it != entry_names.end(); ++it) {
			const std::string &name = *it;
			if (name == "." || name == "..") {
				continue;
			}
			const struct stat &st           = entry_stats.at(name);
			std::string        link         = CreateHyperLink(name + "/");
			std::string        name_padding = CreateSpacePadding(name + "/", kMaxNameWidth);
			std::string        time_stamp   = CreateTimeStamp(st.st_mtim);
			std::string        file_size    = "-";
			std::string        size_padding = CreateSpacePadding(file_size, kMaxFileSizeWidth);
			index += link + name_padding + " " + time_stamp + size_padding + file_size + "\n";
		}
		return index;
	}

	static std::string
	CreateOtherIndex(const EntryNames &entry_names, const EntryStats &entry_stats)
	{
		std::string index;

		for (EntryNames::const_iterator it = entry_names.begin(); it != entry_names.end(); ++it) {
			const std::string &name         = *it;
			const struct stat &st           = entry_stats.at(name);
			std::string        link         = CreateHyperLink(name);
			std::string        name_padding = CreateSpacePadding(name, kMaxNameWidth);
			std::string        time_stamp   = CreateTimeStamp(st.st_mtim);
			std::string        file_size    = utils::ToString(st.st_size);
			std::string        size_padding = CreateSpacePadding(file_size, kMaxFileSizeWidth);
			index += link + name_padding + " " + time_stamp + size_padding + file_size + "\n";
		}
		return index;
	}

	static result::Result<SortedNames> GetSortedDirEntryNames(const std::string &dirname)
	{
		SortedNames entry_names;
		DIR        *dirp = opendir(dirname.c_str());
		if (dirp == NULL) {
			log("autoindex opendir", strerror(errno));
			return err::opendir::GetErrCode(errno);
		}
		while (struct dirent *entry = readdir(dirp)) {
			entry_names.insert(entry->d_name);
		}
		closedir(dirp);
		return entry_names;
	}

	static result::Result<EntryStats>
	GetEntryStats(const std::string &dirname, const SortedNames &entry_names)
	{
		EntryStats stats;

		for (SortedNames::const_iterator it = entry_names.begin(); it != entry_names.end(); ++it) {
			const std::string &name = *it;
			const std::string  path = utils::JoinPath(dirname, name);
			struct stat        st;
			if (stat(path.c_str(), &st) == -1 && lstat(path.c_str(), &st) == -1) {
				log("autoindex stat", strerror(errno));
				return err::stat::GetErrCode(errno);
			}
			stats[name] = st;
		}
		return stats;
	}

	static CategorizedEntry
	CategorizeEntries(const SortedNames &entry_names, const EntryStats &entry_stats)
	{
		CategorizedEntry entries;

		for (SortedNames::const_iterator it = entry_names.begin(); it != entry_names.end(); ++it) {
			const std::string &name = *it;
			const struct stat &st   = entry_stats.at(name);
            S_ISDIR(st.st_mode) ? entries.dirs.push_back(name) : entries.others.push_back(name);
		}
		return entries;
	}

	static std::string CreateHtmlPrefix(const std::string &dirname)
	{
		return "<html>\n"
			   "<head><title>Index of " +
			   dirname +
			   "</title></head>\n"
			   "<body>\n"
			   "<h1>Index of " +
			   dirname + "</h1><hr><pre>";
	}

	static result::Result<std::string> CreateHtmlBody(const std::string &dirname)
	{
		const result::Result<SortedNames> sorted_entries = GetSortedDirEntryNames(dirname);
		if (sorted_entries.IsErr()) {
			return sorted_entries.Err();
		}
		const result::Result<EntryStats> entry_stats = GetEntryStats(dirname, sorted_entries.Val());
		if (entry_stats.IsErr()) {
			return entry_stats.Err();
		}
		const CategorizedEntry entry_names =
			CategorizeEntries(sorted_entries.Val(), entry_stats.Val());
		const std::string dir_index   = CreateDirIndex(entry_names.dirs, entry_stats.Val());
		const std::string other_index = CreateOtherIndex(entry_names.others, entry_stats.Val());
		return dir_index + other_index;
	}

	static std::string CreateHtmlSuffix()
	{
		return "</pre><hr></body>\n"
			   "</html>\n";
	}

	result::Result<std::string>
	CreateAutoIndex(const std::string &root, const std::string &request_path)
	{
		const std::string                 fullpath = utils::JoinPath(root, request_path);
		const result::Result<std::string> body     = CreateHtmlBody(fullpath);
		if (body.IsErr()) {
			return body.Err();
		}
		const std::string prefix = CreateHtmlPrefix(request_path);
		const std::string suffix = CreateHtmlSuffix();
		return prefix + body.Val() + suffix;
	}
} // namespace autoindex
