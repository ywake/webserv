#include "cgi_response.hpp"

#include "http_exceptions.hpp"

namespace cgi
{

	CgiResponse::CgiResponse() {}

	CgiResponse::CgiResponse(const CgiResponse &other) {}

	CgiResponse(server::IRequest &request, conf::LocationConf &location_conf) {}

	Cgi::~Cgi() {}

	/**
	 * IResponse
	 */
	void           Perform(const event::Event &event) {}
	Result<void>   Send(int fd) {}
	bool           HasReadyData() const {}
	bool           HasFd() const {}
	Emptiable<int> GetFd() const {}
	bool           IsFinished() const {}

} // namespace cgi
