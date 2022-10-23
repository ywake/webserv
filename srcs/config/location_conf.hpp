#ifndef LOCATION_CONF_HPP
#define LOCATION_CONF_HPP

#include "config_typedef.hpp"
#include "thin_string.hpp"
#include <map>
#include <string>
#include <vector>

namespace conf
{
	class LocationConf
	{
	  private:
		std::vector<std::string>  allow_methods_;
		std::map<StatusCode, Url> return_;
		Path                      root_;
		std::vector<Path>         index_files_;
		bool                      autoindex;
		Url                       cgi_path_;

	  public:
		LocationConf();
		LocationConf(const std::vector<ThinString> &params);
		~LocationConf();

		std::vector<std::string>  GetAllowMethods();
		std::map<StatusCode, Url> GetReturn();
		Path                      GetRoot();
		std::vector<Path>         GetIndexFiles();
		bool                      GetAutoindex();
		Url                       GetCgiPath();

		bool operator==(const LocationConf &rhs) const;
	};
} // namespace conf

#endif
