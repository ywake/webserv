#include "location_conf.hpp"

#include "config_exceptions.hpp"
#include "webserv_utils.hpp"

namespace conf
{
	LocationConf::LocationConf()
		: allow_methods_(), return_(), root_(), index_files_(), autoindex(), cgi_path_()
	{}

	LocationConf::LocationConf(const std::vector<ThinString> &params)
		: allow_methods_(), return_(), root_(), index_files_(), autoindex(), cgi_path_()
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
					allow_methods_.push_back(it->ToString());
				}
			} else if (split[0] == "return") {
				if (split.size() != 3) {
					throw ConfigException("Invalid config");
				}
				return_[split[1].ToString()] = split[2].ToString();
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
					index_files_.push_back(it->ToString());
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

	std::vector<std::string> LocationConf::GetAllowMethods()
	{
		return allow_methods_;
	}

	std::map<StatusCode, Url> LocationConf::GetReturn()
	{
		return return_;
	}

	Path LocationConf::GetRoot()
	{
		return root_;
	}

	std::vector<Path> LocationConf::GetIndexFiles()
	{
		return index_files_;
	}

	bool LocationConf::GetAutoindex()
	{
		return autoindex;
	}

	Url LocationConf::GetCgiPath()
	{
		return cgi_path_;
	}

	bool LocationConf::operator==(const LocationConf &rhs) const
	{
		return allow_methods_ == rhs.allow_methods_ && return_ == rhs.return_ &&
			   root_ == rhs.root_ && index_files_ == rhs.index_files_ &&
			   autoindex == rhs.autoindex && cgi_path_ == rhs.cgi_path_;
	}
} // namespace conf
