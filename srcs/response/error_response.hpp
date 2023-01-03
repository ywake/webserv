#ifndef ERROR_RESPONSE_HPP
#define ERROR_RESPONSE_HPP

#include "a_response.hpp"

#include "managed_fd.hpp"
#include "server_conf.hpp"
#include "status_code.hpp"

namespace response
{
	class ErrorResponse : public AResponse
	{
	  private:
		const conf::ServerConf &config_;
		ManagedFd               managed_fd_;

	  public:
		ErrorResponse(
			const server::IRequest &request,
			const http::StatusCode &status_code,
			const conf::ServerConf &conf
		);
		FinEventType   Perform(const event::Event &event);
		bool           HasFd() const;
		Emptiable<int> GetFd() const;

	  private:
		Result<void> UseConfiguredErrorPage(const http::StatusCode &code);
	};
} // namespace response

#endif
