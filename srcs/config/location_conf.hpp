#ifndef LOCATION_CONF_HPP
#define LOCATION_CONF_HPP

#include <list>

namespace conf
{
	class LocationConf
	{
	  private:
		std::list<std::string>     allow_methods_;
		std::pair<StatusCode, Url> return_;
		Path                       root_;
		std::list<Path>            index_files_;
		bool                       autoindex;
		Url                        cgi_path_;

	  public:
		LocationConf(/* args */);
		~LocationConf();
	};

	LocationConf::LocationConf(/* args */) {}

	LocationConf::~LocationConf() {}

} // namespace conf

#endif
