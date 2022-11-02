#ifndef LOCATION_CONF_HPP
#define LOCATION_CONF_HPP

#include "conf_elem.hpp"
#include "config_typedef.hpp"
#include "thin_string.hpp"
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace conf
{

	class LocationConf
	{
	  public:
		typedef ConfElem<std::vector<std::string> >   AllowMethods;
		typedef ConfElem<std::pair<StatusCode, Url> > Redirect;
		typedef ConfElem<Path>                        Root;
		typedef ConfElem<std::vector<Path> >          IndexFiles;
		typedef ConfElem<bool>                        AutoIndex;
		typedef ConfElem<std::string>                 CgiPath;

	  private:
		AllowMethods allow_methods_;
		Redirect     redirect_;
		Root         root_;
		IndexFiles   index_files_;
		AutoIndex    autoindex;
		CgiPath      cgi_path_;

	  public:
		LocationConf(const std::vector<ThinString> &params);
		LocationConf(
			AllowMethods allow_methods_ = AllowMethods(),
			Redirect     redirect_      = Redirect(),
			Root         root_          = Root(),
			IndexFiles   index_files_   = IndexFiles(),
			AutoIndex    autoindex      = AutoIndex(),
			CgiPath      cgi_path_      = CgiPath()
		);
		~LocationConf();

		const AllowMethods &GetAllowMethods() const;
		const Redirect     &GetRedirect() const;
		const Root         &GetRoot() const;
		const IndexFiles   &GetIndexFiles() const;
		const AutoIndex    &GetAutoindex() const;
		const CgiPath      &GetCgiPath() const;

		bool                operator==(const LocationConf &rhs) const;
		const LocationConf &operator=(const LocationConf &rhs);

	  private:
		void AddAllowMethods(const std::vector<ThinString> &tokens);
	};

	std::ostream &operator<<(std::ostream &os, const LocationConf &conf);
} // namespace conf

#endif
