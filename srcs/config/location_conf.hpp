#ifndef LOCATION_CONF_HPP
#define LOCATION_CONF_HPP

#include "config_typedef.hpp"
#include "emptiable.hpp"
#include "thin_string.hpp"
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace conf
{
	class LocationConf
	{
	  public:
		enum MatchPattern {
			kPrefix,
			kSuffix,
			kExact
		};

	  public:
		typedef Emptiable<Path>                        PathPattern;
		typedef std::set<std::string>                  AllowMethods;
		typedef Emptiable<std::pair<StatusCode, Url> > Redirect;
		typedef Emptiable<Path>                        Root;
		typedef std::vector<Path>                      IndexFiles;
		typedef Emptiable<bool>                        AutoIndex;
		typedef Emptiable<Path>                        CgiPath;

	  private:
		PathPattern  path_pattern_;
		MatchPattern match_pattern_;
		AllowMethods allow_methods_;
		Redirect     redirect_;
		Root         root_;
		IndexFiles   index_files_;
		AutoIndex    autoindex_;
		CgiPath      cgi_path_;
		const Path  *default_root_;

	  private:
		static const AllowMethods kDefaultAllowMethods;
		static const IndexFiles   kDefaultIndexFiles;
		static const Path         kDefaultRootForTest;

	  public:
		// for test
		LocationConf(
			PathPattern  path_pattern  = PathPattern(),
			MatchPattern match_pattern = kPrefix,
			AllowMethods allow_methods = AllowMethods(),
			Redirect     redirect      = Redirect(),
			Root         root          = Root(),
			IndexFiles   index_files   = IndexFiles(),
			AutoIndex    autoindex     = AutoIndex(),
			CgiPath      cgi_path      = CgiPath(),
			const Path  *default_root  = NULL
		);
		LocationConf(const LocationConf &other);
		LocationConf(
			const PathPattern             &path_pattern,
			MatchPattern                   match_pattern,
			const Path                    &default_root,
			const std::vector<ThinString> &params
		);
		~LocationConf();

		// Methods
		/**
		 * @brief このLocation設定を適用するかどうかを判定する
		 */
		bool IsMatch(const Path &path) const;
		bool IsAllowedMethod(const std::string &method) const;

		// Getters
		const Path         &GetPathPattern() const;
		MatchPattern        GetMatchPattern() const;
		const AllowMethods &GetAllowMethods() const;
		const Redirect     &GetRedirect() const;
		const Path         &GetRoot() const;
		const Root         &GetRawRoot() const;
		const IndexFiles   &GetIndexFiles() const;
		const AutoIndex    &GetAutoindex() const;
		const CgiPath      &GetCgiPath() const;

		// Setters
		void SetDefaultRoot(const Path &root);

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
