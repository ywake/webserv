#include <unistd.h>

#include "a_response.hpp"
#include "body_writer.hpp"
#include "cgi_parser.hpp"
#include "i_request.hpp"
#include "i_response.hpp"
#include "location_conf.hpp"
#include "managed_fd.hpp"
#include "reciever.hpp"
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
			kBody,
			kEnd
		};
		static const int kMaxLoadSize = 8192;

	  private:
		const conf::LocationConf &location_conf_;
		Path                      resource_path_;
		ManagedFd                 parent_fd_;
		ManagedFd                 child_fd_;
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
		CgiResponse::Path         GetResourcePath() const;
		Result<CgiResponse::Path> FindResourcePath() const;
		Result<CgiResponse::Path> GetAccessiblePath(const CgiResponse::Path &path) const;
		std::vector<Path>         CombineIndexFiles(const Path &base_path) const;
		Result<CgiResponse::Path> FindAccessiblePathFromArray(const std::vector<Path> &candidates
		) const;
		Result<http::StatusCode>  ParseStatusCode(const http::FieldSection::Values &values);
		Result<void>              PushMetaDataToSendBody(const http::FieldSection &field_section);
		void                      PushMetaDataForClientRedirect(const std::string &uri);
		void                      ThrowLocalRedirect(const http::FieldSection &field_section);

		void                               OnWriteReady();
		void                               ExecCgi();
		void                               ChildProcess();
		void                               ParentProcess(pid_t pid);
		Result<std::vector<const char *> > CreateArgs();
		Result<std::vector<const char *> > CreateEnvs();
		void                               SetMetaEnv(std::vector<const char *> &envs);
		void StoreHeadersToSendBody(const http::FieldSection &field_section);

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
