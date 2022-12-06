#include "cgi_response.hpp"

#include "http_exceptions.hpp"

namespace cgi
{
	CgiResponse::CgiResponse(const CgiResponse &other)
		: request_(other.request_), location_conf_(other.location_conf_)
	{}

	CgiResponse(const server::IRequest &request, const conf::LocationConf &location_conf)
		: request_(request), location_conf_(location_conf)
	{}

	CgiResponse::~CgiResponse() {}

	CgiResponse &CgiResponse::operator=(const CgiResponse &other)
	{
		if (this == &other) {
			return *this;
		}
		request_       = other.request_;
		location_conf_ = other.location_conf_;
		return *this;
	}

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
