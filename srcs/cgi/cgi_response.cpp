#include "cgi_response.hpp"

#include "http_exceptions.hpp"

namespace cgi
{
	CgiResponse::CgiResponse(const CgiResponse &other)
		: request_(other.request_),
		  location_conf_(other.location_conf_),
		  resource_path_(other.resource_path_)
	{}

	CgiResponse::CgiResponse(server::IRequest &request, conf::LocationConf &location_conf)
		: request_(request), location_conf_(location_conf), resource_path_()
	{}

	CgiResponse::~CgiResponse() {}

	CgiResponse &CgiResponse::operator=(const CgiResponse &other)
	{
		if (this == &other) {
			return *this;
		}
		request_       = other.request_;
		location_conf_ = other.location_conf_;
		resource_path_ = other.resource_path_;
		return *this;
	}

	/**
	 * IResponse
	 */
	void           CgiResponse::Perform(const event::Event &event) {}
	Result<void>   CgiResponse::Send(int fd) {}
	bool           CgiResponse::HasReadyData() const {}
	bool           CgiResponse::HasFd() const {}
	Emptiable<int> CgiResponse::GetFd() const {}
	std::size_t    CgiResponse::size() const {};
	bool           CgiResponse::IsFinished() const {}

} // namespace cgi
