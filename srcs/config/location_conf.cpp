#include "location_conf.hpp"

#include "config_exceptions.hpp"
#include "webserv_utils.hpp"

namespace conf
{
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
			std::vector<ThinString> split = Split(*it, " ");
			if (split.size() >= 2) {
				throw ConfigException("Invalid config");
			}
			if (split[0] == "allow_methods") {
				if (split.size() != 2) {
					throw ConfigException("Invalid config");
				}
				for (std::vector<ThinString>::iterator it = split.begin() + 1; it != split.end();
					 ++it) {
					allow_methods_.Value().push_back(it->ToString());
				}
			} else if (split[0] == "return") {
				if (split.size() != 3) {
					throw ConfigException("Invalid config");
				}
				redirect_.Value() =
					std::pair<StatusCode, Url>(split[1].ToString(), split[2].ToString());
			} else if (split[0] == "root") {
				if (split.size() != 2) {
					throw ConfigException("Invalid config");
				}
				root_ = split[1].ToString();
			} else if (split[0] == "index_files") {
				if (split.size() != 2) {
					throw ConfigException("Invalid config");
				}
				for (std::vector<ThinString>::iterator it = split.begin() + 1; it != split.end();
					 ++it) {
					index_files_.Value().push_back(it->ToString());
				}
			} else if (split[0] == "autoindex") {
				if (split.size() != 2) {
					throw ConfigException("Invalid config");
				}
				autoindex = split[1].ToString() == "on";
			} else if (split[0] == "cgi_path") {
				if (split.size() != 2) {
					throw ConfigException("Invalid config");
				}
				cgi_path_ = split[1].ToString();
			} else {
				throw ConfigException("Invalid config");
			}
		}
	}

	LocationConf::~LocationConf() {}

	const LocationConf::AllowMethods &LocationConf::GetAllowMethods() const
	{
		return allow_methods_;
	}

	const LocationConf::Redirect &LocationConf::GetRedirect() const
	{
		return redirect_;
	}

	const LocationConf::Root &LocationConf::GetRoot() const
	{
		return root_;
	}

	const LocationConf::IndexFiles &LocationConf::GetIndexFiles() const
	{
		return index_files_;
	}

	const LocationConf::AutoIndex &LocationConf::GetAutoindex() const
	{
		return autoindex;
	}

	const LocationConf::CgiPath &LocationConf::GetCgiPath() const
	{
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
			std::vector<std::string> allow_methods_value = allow_methods.Value();
			for (std::vector<std::string>::const_iterator it = allow_methods_value.begin();
				 it != allow_methods_value.end();
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
			std::vector<std::string> index_files_value = index_files.Value();
			for (std::vector<std::string>::const_iterator it = index_files_value.begin();
				 it != index_files_value.end();
				 ++it) {
				os << *it << " ";
			}
		}
		os << "], ";

		LocationConf::AutoIndex auto_index = conf.GetAutoindex();
		os << "autoindex: " << (auto_index.empty() ? "false" : (auto_index.Value() ? "on" : "off"))
		   << ", ";

		LocationConf::CgiPath cgi_path = conf.GetCgiPath();
		os << "cgi_path: " << (cgi_path.empty() ? "" : cgi_path.Value()) << std::endl;
		return os;
	}
} // namespace conf
