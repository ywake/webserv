#ifndef CGI_RESPONSE_HPP
#define CGI_RESPONSE_HPP

#include <netinet/in.h>
#include <unistd.h>

#include "a_response.hpp"
#include "body_writer.hpp"
#include "cgi_parser.hpp"
#include "i_request.hpp"
#include "location_conf.hpp"
#include "managed_fd.hpp"
#include "meta_env.hpp"
#include "receiver.hpp"
#include "resolve_index_file.hpp"
#include "stat.hpp"
#include "string_array.hpp"

namespace cgi
{
	class CgiResponse : public response::AResponse
	{
	  public:
		typedef std::string Path;

	  public:
		static const std::string kCgiVersion;

	  private:
		enum State {
			kHeader,
			kBody
		};
		static const time_t kLifeTimeSec;

	  private:
		const conf::LocationConf &location_conf_;
		ManagedFd                 parent_fd_;
		server::BodyWriter        body_writer_;
		server::Reciever          cgi_receiver_;
		cgi::CgiParser            field_parser_;
		State                     state_;
		pid_t                     pid_;
		struct ::timespec         time_;

	  private:
		CgiResponse(const CgiResponse &other);
		CgiResponse &operator=(const CgiResponse &other);

	  public:
		CgiResponse(
			server::IRequest              &request,
			const conf::LocationConf      &location_conf,
			const struct sockaddr_storage *server,
			const struct sockaddr_storage *client
		);
		~CgiResponse();

		// Methods
	  private:
		Result<void>          CreateUds(ManagedFd &parent_fd, ManagedFd &child_fd);
		std::string           TrimPathInfo(const std::string &request_path);
		response::PartialPath TryResolveIndexFilePath(
			const response::FullPath             &root,
			const response::PartialPath          &request_path,
			const conf::LocationConf::IndexFiles &index_files
		);
		Stat                              TryStat(const std::string &path) const;
		Result<std::vector<std::string> > ParseQuery(const std::string &query);
		Result<http::StatusCode>          ParseStatusCode(const http::FieldSection::Values &values);
		Result<void> PushMetaDataToSendBody(const http::FieldSection &field_section);
		void         PushMetaDataForClientRedirect(const std::string &uri);
		void         ThrowLocalRedirect(const http::FieldSection &field_section);
		void         ExecuteDirectoryRedirect(const std::string &request_path);

		std::string              CreateLocationUrl(const std::string &path) const;
		std::vector<std::string> CreateCgiArgs(
			const std::string              &cgi_path,
			const std::string              &script_path,
			const std::vector<std::string> &querys
		);
		Result<StringArray> CreateEnvVariables(
			const std::string             &script_name,
			const struct sockaddr_storage *server,
			const struct sockaddr_storage *client
		);
		Result<void> ExecCgi(ManagedFd &child_fd, const StringArray &args, const StringArray &envs);
		void ExecChild(ManagedFd &child_fd, const StringArray &args, const StringArray &envs);

		void StoreHeadersToSendBody(const http::FieldSection &field_section);

		bool IsLocalRedirect(const http::FieldSection &field_section) const;
		bool IsClientRedirect(const http::FieldSection &field_section) const;

		response::AResponse::FinEventType OnWriteReady();
		response::AResponse::FinEventType OnReadReady();
		void                              OnHeader();
		void                              OnBody();

		// IResponse
	  public:
		FinEventType   Perform(const event::Event &event);
		bool           HasFd() const;
		Emptiable<int> GetFd() const;

		// CgiResponse &operator=(const CgiResponse &other);
	};

} // namespace cgi

#endif
