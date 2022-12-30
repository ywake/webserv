#include "location_conf.hpp"

#include <cassert>

#include "config_exceptions.hpp"
#include "webserv_utils.hpp"

namespace conf
{
	// AllowMethods({"GET", "POST", "DELETE"})がC++98で使えないので
	static const char               *kDefaultAllowMethodArray[] = {"GET"};
	const LocationConf::AllowMethods LocationConf::kDefaultAllowMethods =
		LocationConf::AllowMethods(
			kDefaultAllowMethodArray,
			kDefaultAllowMethodArray + ARRAY_SIZE(kDefaultAllowMethodArray)
		);
	// IndexFiles({"index.html"})がC++98で使えないので
	static const char             *kDefaultIndexFileArray[]         = {"index.html"};
	const LocationConf::IndexFiles LocationConf::kDefaultIndexFiles = LocationConf::IndexFiles(
		kDefaultIndexFileArray, kDefaultIndexFileArray + ARRAY_SIZE(kDefaultIndexFileArray)
	);

	LocationConf::LocationConf(
		PathPattern  path_pattern,
		MatchPattern match_pattern,
		AllowMethods allow_methods,
		Redirect     redirect,
		Root         root,
		IndexFiles   index_files,
		AutoIndex    autoindex,
		CgiPath      cgi_path,
		const Path  *default_root
	)
		: path_pattern_(path_pattern),
		  match_pattern_(match_pattern),
		  allow_methods_(allow_methods),
		  redirect_(redirect),
		  root_(root),
		  index_files_(index_files),
		  autoindex_(autoindex),
		  cgi_path_(cgi_path),
		  default_root_(default_root)
	{}

	LocationConf::LocationConf(const LocationConf &other)
	{
		*this = other;
	}

	LocationConf::LocationConf(
		const PathPattern             &path_pattern,
		MatchPattern                   match_pattern,
		const Path                    &default_root,
		const std::vector<ThinString> &params
	)
		: path_pattern_(path_pattern),
		  match_pattern_(match_pattern),
		  allow_methods_(),
		  redirect_(),
		  root_(),
		  index_files_(),
		  autoindex_(),
		  cgi_path_(),
		  default_root_(&default_root)
	{
		for (std::vector<ThinString>::const_iterator it = params.begin(); it != params.end();
			 ++it) {
			std::vector<ThinString> split = utils::TrimEmpty(Split(*it, " "));
			if (split.size() < 2) {
				throw ConfigException("Invalid location config: invalid size");
			}
			if (split[0] == "allow_methods") {
				AddAllowMethods(split);
			} else if (split[0] == "redirect") {
				AddRedirect(split);
			} else if (split[0] == "root") {
				AddRoot(split);
			} else if (split[0] == "index") {
				AddIndexFiles(split);
			} else if (split[0] == "autoindex") {
				AddAutoIndex(split);
			} else if (split[0] == "cgi_path") {
				AddCgiPath(split);
			} else {
				throw ConfigException("Invalid location config: invalid directive");
			}
		}
	}

	void LocationConf::AddAllowMethods(const std::vector<ThinString> &tokens)
	{
		for (std::vector<ThinString>::const_iterator it = tokens.begin() + 1; it != tokens.end();
			 ++it) {
			// TODO: METHODSのプールを用意する
			if (*it != "GET" && *it != "POST" && *it != "DELETE") {
				throw ConfigException("Invalid allow_methods");
			}
			allow_methods_.insert(it->ToString());
		}
	}

	void LocationConf::AddRedirect(const std::vector<ThinString> &tokens)
	{
		if (tokens.size() != 3) {
			throw ConfigException("Invalid redirect");
		}
		if (tokens[1] != "301" && tokens[1] != "302" && tokens[1] != "303" && tokens[1] != "307" &&
			tokens[1] != "308") {
			throw ConfigException("Invalid redirect");
		}
		redirect_ = std::pair<StatusCode, Url>(tokens[1].ToString(), tokens[2].ToString());
	}

	void LocationConf::AddRoot(const std::vector<ThinString> &tokens)
	{
		if (tokens.size() != 2) {
			throw ConfigException("Invalid root");
		}
		root_ = tokens[1].ToString();
	}

	void LocationConf::AddIndexFiles(const std::vector<ThinString> &tokens)
	{
		for (std::vector<ThinString>::const_iterator it = tokens.begin() + 1; it != tokens.end();
			 ++it) {
			index_files_.push_back(it->ToString());
		}
	}

	void LocationConf::AddAutoIndex(const std::vector<ThinString> &tokens)
	{
		if (tokens.size() != 2) {
			throw ConfigException("Invalid autoindex");
		}

		if (tokens[1] == "on") {
			autoindex_ = true;
		} else if (tokens[1] == "off") {
			autoindex_ = false;
		} else {
			throw ConfigException("Invalid autoindex");
		}
	}

	void LocationConf::AddCgiPath(const std::vector<ThinString> &tokens)
	{
		if (tokens.size() != 2) {
			throw ConfigException("Invalid cgi_path");
		}
		cgi_path_ = tokens[1].ToString();
	}

	LocationConf::~LocationConf() {}

	/**
	 * Methods
	 */
	bool LocationConf::IsMatch(const Path &path) const
	{
		assert(!path_pattern_.empty());
		switch (match_pattern_) {
		case kPrefix:
			return path.find(path_pattern_.Value()) == 0;
		case kSuffix:
			return utils::EndWith(path, path_pattern_.Value());
			// return path.rfind(path_pattern_) == path.size() - path_pattern_.size();
		case kExact:
			return path == path_pattern_.Value();
		default:
			return false;
		}
	}

	bool LocationConf::IsAllowedMethod(const std::string &method) const
	{
		if (allow_methods_.empty()) {
			return kDefaultAllowMethods.find(method) != kDefaultAllowMethods.end();
		}
		return allow_methods_.find(method) != allow_methods_.end();
	}

	/**
	 * Getters
	 */
	const Path &LocationConf::GetPathPattern() const
	{
		assert(!path_pattern_.empty());
		return path_pattern_.Value();
	}

	LocationConf::MatchPattern LocationConf::GetMatchPattern() const
	{
		return match_pattern_;
	}

	const LocationConf::AllowMethods &LocationConf::GetAllowMethods() const
	{
		if (allow_methods_.empty()) {
			return kDefaultAllowMethods;
		}
		return allow_methods_;
	}

	const LocationConf::Redirect &LocationConf::GetRedirect() const
	{
		return redirect_;
	}

	const Path &LocationConf::GetRoot() const
	{
		if (root_.empty()) {
			return *default_root_;
		}
		return root_.Value();
	}
	const LocationConf::Root &LocationConf::GetRawRoot() const
	{
		return root_;
	}

	const LocationConf::IndexFiles &LocationConf::GetIndexFiles() const
	{
		if (index_files_.empty()) {
			return kDefaultIndexFiles;
		}
		return index_files_;
	}

	const LocationConf::AutoIndex &LocationConf::GetAutoindex() const
	{
		return autoindex_;
	}

	const LocationConf::CgiPath &LocationConf::GetCgiPath() const
	{
		return cgi_path_;
	}

	/**
	 * Setters
	 */
	void LocationConf::SetDefaultRoot(const Path &path)
	{
		default_root_ = &path;
	}

	/**
	 * Operators
	 */
	bool LocationConf::operator==(const LocationConf &rhs) const
	{
		return path_pattern_ == rhs.path_pattern_ && match_pattern_ == rhs.match_pattern_ &&
			   allow_methods_ == rhs.allow_methods_ && redirect_ == rhs.redirect_ &&
			   root_ == rhs.root_ && index_files_ == rhs.index_files_ &&
			   autoindex_ == rhs.autoindex_ && cgi_path_ == rhs.cgi_path_;
	}

	const LocationConf &LocationConf::operator=(const LocationConf &rhs)
	{
		if (this == &rhs) {
			return *this;
		}
		path_pattern_  = rhs.path_pattern_;
		match_pattern_ = rhs.match_pattern_;
		allow_methods_ = rhs.allow_methods_;
		redirect_      = rhs.redirect_;
		root_          = rhs.root_;
		index_files_   = rhs.index_files_;
		autoindex_     = rhs.autoindex_;
		cgi_path_      = rhs.cgi_path_;
		default_root_  = rhs.default_root_;
		return *this;
	}

	std::ostream &operator<<(std::ostream &os, const LocationConf &conf)
	{
		os << "path_pattern: " << conf.GetPathPattern() << ", ";

		os << "match_pattern: ";
		switch (conf.GetMatchPattern()) {
		case LocationConf::kPrefix:
			os << "^";
			break;
		case LocationConf::kSuffix:
			os << "$";
			break;
		case LocationConf::kExact:
			os << "=";
			break;
		}
		os << ", ";

		os << "allow_methods: [ ";
		LocationConf::AllowMethods allow_methods = conf.GetAllowMethods();
		if (!allow_methods.empty()) {
			for (LocationConf::AllowMethods::const_iterator it = allow_methods.begin();
				 it != allow_methods.end();
				 ++it) {
				os << *it << " ";
			}
		}
		os << "], ";

		os << "return: ";
		LocationConf::Redirect redirect = conf.GetRedirect();
		if (!redirect.empty()) {
			std::pair<StatusCode, Url> redirect_value = redirect.Value();
			os << redirect_value.first << " " << redirect_value.second;
		}
		os << ", ";

		LocationConf::Root root = conf.GetRawRoot();
		os << "root: " << (root.empty() ? "" : root.Value()) << ", ";

		os << "index_files: [ ";
		LocationConf::IndexFiles index_files = conf.GetIndexFiles();
		if (!index_files.empty()) {
			for (LocationConf::IndexFiles::const_iterator it = index_files.begin();
				 it != index_files.end();
				 ++it) {
				os << *it << " ";
			}
		}
		os << "], ";

		LocationConf::AutoIndex auto_index = conf.GetAutoindex();
		os << "autoindex: " << (auto_index.empty() ? "false" : (auto_index.Value() ? "on" : "off"))
		   << ", ";

		LocationConf::CgiPath cgi_path = conf.GetCgiPath();
		os << "cgi_path: " << (cgi_path.empty() ? "" : cgi_path.Value());
		return os;
	}
} // namespace conf
