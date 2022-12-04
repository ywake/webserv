#ifndef RESPONSE_HOLDER_HPP
#define RESPONSE_HOLDER_HPP

#include <deque>
#include <utility>

#include "i_request.hpp"
#include "i_response.hpp"
#include "instruction.hpp"
#include "result.hpp"
#include "status_code.hpp"
#include "virtual_server_confs.hpp"

namespace server
{
	class ResponseHolder
	{
		typedef void (*RequestDelFunc)(IRequest *);
		typedef std::pair<IRequest *, IResponse *> ReqRes;

	  private:
		RequestDelFunc                  request_del_;
		int                             conn_fd_;
		const conf::VirtualServerConfs &config_;
		std::deque<ReqRes>              in_progress_;

	  public:
		ResponseHolder(const conf::VirtualServerConfs &conf, RequestDelFunc del);
		event::Instructions         StartNewResponse(IRequest *request);
		event::Instructions         Perform(const event::Event &event);
		Result<event::Instructions> Send();
		std::size_t                 size();

	  private:
		event::Instructions FinishFrontResponse();
		IResponse          *CreateNewResponse(const IRequest &request);
		// IResponse *CreateCgiResponse(const IRequest &request);
		IResponse *
		CreateErrorResponse(const http::StatusCode &status_code, IRequest::ErrorType e_type);
		event::Instructions CreateInstructionsForNewResopnse(const IResponse &response);
	};
} // namespace server

#endif
