#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <vector>

template <typename T>
std::string ToString(const T &data)
{
	std::stringstream ss;
	ss << data;
	return ss.str();
}

bool IsVchar(char c)
{
	return 0x21 <= c && c <= 0x7e;
}

std::map<unsigned char, std::string> InitHtmlEscape();

static const std::map<unsigned char, std::string> kHtmlEscape = InitHtmlEscape();

static const std::size_t kMaxNameWidth = 50;
static const std::size_t kMaxSizeWidth = 20;

typedef std::set<std::string>              EntryNames;
typedef std::map<std::string, struct stat> EntryStats;
typedef EntryNames::const_iterator         NamesItr;

typedef struct CategorizedEntry {
	std::vector<std::string> dirs;
	std::vector<std::string> others;
} CategorizedEntry;

std::map<unsigned char, std::string> InitHtmlEscape()
{
	std::map<unsigned char, std::string> m;
	m['"'] = "&quot;";
	m['&'] = "&amp;";
	m['<'] = "&lt;";
	m['>'] = "&gt;";
	return m;
}

static const std::string kNeedToEncodes = " !&#$%\"'()*+,:;<=>?@[\\]^`{|}";

std::string PercentEncode(const std::string &s)
{
	std::stringstream ss;

	for (std::size_t i = 0; i < s.size(); i++) {
		unsigned char c = s[i];
		if (!IsVchar(c) || kNeedToEncodes.find(c) != std::string::npos) {
			ss << "%" << std::hex << std::uppercase << (size_t)c;
			continue;
		}
		ss << c;
	}
	return ss.str();
}

std::string ReplaceHtmlReserved(const std::string &s)
{
	std::string replaced;

	typedef std::map<unsigned char, std::string>::const_iterator MapItr;
	for (size_t i = 0; i < s.size(); i++) {
		unsigned char c  = s[i];
		MapItr        it = kHtmlEscape.find(c);
		if (it == kHtmlEscape.end()) {
			replaced += c;
		} else {
			replaced += kHtmlEscape.at(c);
		}
	}
	return replaced;
}

std::string OmitExcessBytes(const std::string &s)
{
	if (s.size() <= kMaxNameWidth) {
		return s;
	}
	static const std::string kEllipsis = "..>";
	return s.substr(0, kMaxNameWidth - kEllipsis.size()) + kEllipsis;
}

std::string CreateHyperLink(const std::string &path)
{
	std::string link_path = PercentEncode(path);
	std::string omitted   = OmitExcessBytes(path);
	std::string link_name = ReplaceHtmlReserved(omitted);
	std::string link      = "<a href=\"" + link_path + "\">" + link_name + "</a>";
	return link;
}

std::string CreateSpacePadding(const std::string &s, std::size_t max_width)
{
	if (s.size() >= max_width) {
		return "";
	}
	return std::string(max_width - s.size(), ' ');
}

// "18-Dec-2022 03:36";
std::string CreateTimeStamp(const struct timespec &ts)
{
	struct tm tm;
	char      buf[1000];

	localtime_r(&ts.tv_sec, &tm); // TODO error
	strftime(buf, sizeof(buf), "%d-%b-%Y %R", &tm);
	return std::string(buf);
}

std::string CreateIndexLine(const std::string &basename, const struct stat &st)
{
	std::string link         = CreateHyperLink(basename);
	std::string name_padding = CreateSpacePadding(basename, kMaxNameWidth);
	std::string time_stamp   = CreateTimeStamp(st.st_mtim);
	std::string file_size    = ToString(st.st_size);
	std::string size_padding = CreateSpacePadding(basename, kMaxSizeWidth);

	return link + name_padding + time_stamp + size_padding + file_size + "\n";
}

EntryNames GetSortedDirEntryNames(const std::string &dirname)
{
	EntryNames entry_names;
	DIR       *dirp = opendir(dirname.c_str()); // TODO error
	while (struct dirent *entry = readdir(dirp)) {
		entry_names.insert(entry->d_name);
	}
	closedir(dirp);
	return entry_names;
}

EntryStats GetEntryStats(const std::string &dirname, const EntryNames &entry_names)
{
	EntryStats stats;

	for (NamesItr it = entry_names.begin(); it != entry_names.end(); ++it) {
		const std::string &name = *it;
		const std::string  path = dirname + "/" + name; // TODO path join
		struct stat        st;
		errno = 0;
		if (stat(path.c_str(), &st) == -1 && lstat(path.c_str(), &st) == -1) {
			printf("%s: %s\n", path.c_str(), strerror(errno));
		} // TODO error
		stats[name] = st;
	}
	return stats;
}

CategorizedEntry CategorizeEntries(const EntryNames &entry_names, const EntryStats &entry_stats)
{
	CategorizedEntry entries;

	for (NamesItr it = entry_names.begin(); it != entry_names.end(); ++it) {
		const std::string &name = *it;
		const struct stat &st   = entry_stats.at(name);
        S_ISDIR(st.st_mode) ? entries.dirs.push_back(name) : entries.others.push_back(name);
	}
	return entries;
}

// std::string
// CreateDirIndex(const std::vector<std::string> &entry_names, const EntryStats &entry_stats)
// {}

// std::string
// CreateOtherIndex(const std::vector<std::string> &entry_names, const EntryStats &entry_stats)
// {}

int main(int argc, char **argv)
{
	if (argc < 2) {
		return 0;
	}
	const std::string      dirname        = argv[1];
	const EntryNames       sorted_entries = GetSortedDirEntryNames(dirname);
	const EntryStats       entry_stats    = GetEntryStats(dirname, sorted_entries);
	const CategorizedEntry entry_names    = CategorizeEntries(sorted_entries, entry_stats);

	// for (std::vector<std::string>::const_iterator it = entry_names.dirs.begin();
	// 	 it != entry_names.dirs.end();
	// 	 ++it) {
	// 	std::cout << CreateIndexLine(*it, entry_stats.at(*it));
	// }
	for (std::vector<std::string>::const_iterator it = entry_names.others.begin();
		 it != entry_names.others.end();
		 ++it) {
		std::cout << CreateIndexLine(*it, entry_stats.at(*it));
	}
}

// char *s = "<html>\n"
// 		  "<head><title>Index of /c/</title></head>\n"
// 		  "<body>\n"
// 		  "<h1>Index of /c/</h1><hr><pre>";

// char *s2 = "</pre><hr></body>\n"
// 		   "</html>\n";
