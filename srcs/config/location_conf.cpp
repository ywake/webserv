#include "location_conf.hpp"

#include "config_exceptions.hpp"
#include "webserv_utils.hpp"

namespace conf
{
	const LocationConf::Redirect  LocationConf::kDefaultRedirect  = LocationConf::Redirect();
	const LocationConf::Root      LocationConf::kDefaultRoot      = LocationConf::Root();
	const LocationConf::AutoIndex LocationConf::kDefaultAutoIndex = false;
	const LocationConf::CgiPath   LocationConf::kDefaultCgiPath   = LocationConf::CgiPath();
	// AllowMethods({"GET", "POST", "DELETE"})がC++98で使えないので
	static const char               *kDefaultAllowMethodArray[] = {"GET", "POST", "DELETE"};
	const LocationConf::AllowMethods LocationConf::kDefaultAllowMethods =
		LocationConf::AllowMethods(
			kDefaultAllowMethodArray,
			kDefaultAllowMethodArray +
				sizeof(kDefaultAllowMethodArray) / sizeof(kDefaultAllowMethodArray[0])
		);
	// IndexFiles({"index.html"})がC++98で使えないので
	static const char             *kDefaultIndexFileArray[]         = {"index.html"};
	const LocationConf::IndexFiles LocationConf::kDefaultIndexFiles = LocationConf::IndexFiles(
		kDefaultIndexFileArray,
		kDefaultIndexFileArray + sizeof(kDefaultIndexFileArray) / sizeof(kDefaultIndexFileArray[0])
	);

	LocationConf::LocationConf(
		AllowMethods allow_methods_,
		Redirect     redirect_,
		Root         root_,
		IndexFiles   index_files_,
		AutoIndex    autoindex,
		CgiPath      cgi_path_
	)
		: allow_methods_(allow_methods_),
		  redirect_(redirect_),
		  root_(root_),
		  index_files_(index_files_),
		  autoindex(autoindex),
		  cgi_path_(cgi_path_)
	{}

	LocationConf::LocationConf(const std::vector<ThinString> &params)
		: allow_methods_(), redirect_(), root_(), index_files_(), autoindex(), cgi_path_()
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
			} else if (split[0] == "index_files") {
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
			allow_methods_.push_back(it->ToString());
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
			autoindex = true;
		} else if (tokens[1] == "off") {
			autoindex = false;
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

	const LocationConf::AllowMethods &LocationConf::GetAllowMethods() const
	{
		if (allow_methods_.empty()) {
			return kDefaultAllowMethods;
		}
		return allow_methods_;
	}

	const LocationConf::Redirect &LocationConf::GetRedirect() const
	{
		if (redirect_.empty()) {
			return kDefaultRedirect;
		}
		return redirect_;
	}

	const LocationConf::Root &LocationConf::GetRoot() const
	{
		if (root_.empty()) {
			return kDefaultRoot;
		}
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
		if (autoindex.empty()) {
			return kDefaultAutoIndex;
		}
		return autoindex;
	}

	const LocationConf::CgiPath &LocationConf::GetCgiPath() const
	{
		if (cgi_path_.empty()) {
			return kDefaultCgiPath;
		}
		return cgi_path_;
	}

	bool LocationConf::operator==(const LocationConf &rhs) const
	{
		return allow_methods_ == rhs.allow_methods_ && redirect_ == rhs.redirect_ &&
			   root_ == rhs.root_ && index_files_ == rhs.index_files_ &&
			   autoindex == rhs.autoindex && cgi_path_ == rhs.cgi_path_;
	}

	const LocationConf &LocationConf::operator=(const LocationConf &rhs)
	{
		if (this != &rhs) {
			allow_methods_ = rhs.allow_methods_;
			redirect_      = rhs.redirect_;
			root_          = rhs.root_;
			index_files_   = rhs.index_files_;
			autoindex      = rhs.autoindex;
			cgi_path_      = rhs.cgi_path_;
		}
		return *this;
	}

	std::ostream &operator<<(std::ostream &os, const LocationConf &conf)
	{
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

		LocationConf::Root root = conf.GetRoot();
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
