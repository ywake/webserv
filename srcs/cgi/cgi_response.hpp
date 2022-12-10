#include "i_request.hpp"
#include "i_response.hpp"
#include "location_conf.hpp"
#include "stat.hpp"

namespace cgi
{
	class CgiResponse : public server::IResponse
	{
	  public:
		typedef std::string Path;

	  private:
		server::IRequest   &request_;
		conf::LocationConf &location_conf_;
		Path                resource_path_;

	  public:
		CgiResponse(const CgiResponse &other);
		CgiResponse(server::IRequest &request, conf::LocationConf &location_conf);
		~CgiResponse();

		// Methods
	  private:
		void GetResourcePathCandidates(std::vector<CgiResponse::Path> &candidates) const;
		std::vector<Path> CombineIndexFiles(const Path &base_path) const;
		Result<Path>      FindAccessible(const std::vector<Path> &candidates) const;
		void              CgiStatErrorHandler(const result::ErrCode &error) const;
		void              CgiStatFileTypeHandler(const Stat &stat) const;
		// IResponse
	  public:
		void                Perform(const event::Event &event);
		Result<void>        Send(int fd);
		bool                HasReadyData() const;
		bool                HasFd() const;
		Emptiable<int>      GetFd() const;
		virtual std::size_t size() const;
		bool                IsFinished() const;

		CgiResponse &operator=(const CgiResponse &other);
	};

} // namespace cgi
