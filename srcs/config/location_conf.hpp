#ifndef LOCATION_CONF_HPP
#define LOCATION_CONF_HPP

#include "config_typedef.hpp"
#include "emptiable.hpp"
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
		typedef std::vector<std::string>               AllowMethods;
		typedef Emptiable<std::pair<StatusCode, Url> > Redirect;
		typedef Emptiable<Path>                        Root;
		typedef std::vector<Path>                      IndexFiles;
		typedef Emptiable<bool>                        AutoIndex;
		typedef Emptiable<Path>                        CgiPath;

	  private:
		AllowMethods allow_methods_;
		Redirect     redirect_;
		Root         root_;
		IndexFiles   index_files_;
		AutoIndex    autoindex;
		CgiPath      cgi_path_;

	  private:
		static AllowMethods    kDefaultAllowMethods;
		static const Redirect  kDefaultRedirect;
		static const Root      kDefaultRoot;
		static IndexFiles      kDefaultIndexFiles;
		static const AutoIndex kDefaultAutoIndex;
		static const CgiPath   kDefaultCgiPath;
		void                   SetDefault();

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
		void AddRedirect(const std::vector<ThinString> &tokens);
		void AddRoot(const std::vector<ThinString> &tokens);
		void AddIndexFiles(const std::vector<ThinString> &tokens);
		void AddAutoIndex(const std::vector<ThinString> &tokens);
		void AddCgiPath(const std::vector<ThinString> &tokens);
	};

	std::ostream &operator<<(std::ostream &os, const LocationConf &conf);
} // namespace conf

#endif
