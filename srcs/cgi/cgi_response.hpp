#ifndef CGI_RESPONSE_HPP
#define CGI_RESPONSE_HPP

#include <unistd.h>

#include "a_response.hpp"
#include "body_writer.hpp"
#include "cgi_parser.hpp"
#include "i_request.hpp"
#include "i_response.hpp"
#include "location_conf.hpp"
#include "managed_fd.hpp"
#include "receiver.hpp"
#include "resolve_index_file.hpp"
#include "stat.hpp"

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
		static const int kMaxLoadSize = 8192;

	  private:
		const conf::LocationConf &location_conf_;
		ManagedFd                 parent_fd_;
		server::BodyWriter        body_writer_;
		server::Reciever          cgi_receiver_;
		cgi::CgiParser            field_parser_;
		State                     state_;
		pid_t                     pid_;

	  private:
		CgiResponse(const CgiResponse &other);
		CgiResponse &operator=(const CgiResponse &other);

	  public:
		CgiResponse(server::IRequest &request, const conf::LocationConf &location_conf);
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
		Result<http::StatusCode> ParseStatusCode(const http::FieldSection::Values &values);
		Result<void>             PushMetaDataToSendBody(const http::FieldSection &field_section);
		void                     PushMetaDataForClientRedirect(const std::string &uri);
		void                     ThrowLocalRedirect(const http::FieldSection &field_section);

		void                     OnWriteReady();
		Result<void>             ExecCgi(const std::string &script_name, ManagedFd &child_fd);
		void                     ExecChild(const std::string &script_name, ManagedFd &child_fd);
		void                     ExecParent(pid_t pid);
		std::vector<std::string> CreateArgs(
			const std::string &cgi_path, const std::string &script_name, const std::string &query
		);
		std::vector<std::string> CreateEnvs();
		void                     SetMetaEnv(std::vector<const char *> &envs);
		void                     StoreHeadersToSendBody(const http::FieldSection &field_section);

		bool IsLocalRedirect(const http::FieldSection &field_section) const;
		bool IsClientRedirect(const http::FieldSection &field_section) const;

		void OnHeader();
		void OnBody();

		void OnReadReady();

		// IResponse
	  public:
		void           Perform(const event::Event &event);
		bool           HasFd() const;
		Emptiable<int> GetFd() const;

		// CgiResponse &operator=(const CgiResponse &other);
	};

} // namespace cgi

#endif
