#include "response_holder.hpp"
#include "error_response.hpp"
#include "http_exceptions.hpp"
#include "static_response.hpp"

namespace server
{
	inline const std::string &GetHost(const IRequest &request)
	{
		return request.Headers()["host"].front().GetValue();
	}

	event::Instructions ResponseHolder::StartNewResponse(IRequest *request)
	{

		in_progress_.push_back(ReqRes(request, NULL));
		if (request->IsErr()) {
			in_progress_.back().second =
				new ErrorResponse(request->GetErrStatusCode(), config_[GetHost(*request)]);
			event::Instructions insts;
			insts.push_back(event::Instruction(
				event::Instruction::kAppendEventType, conn_fd_, event::Event::kWrite
			));
			return insts;
		}
		IResponse *new_response    = CreateNewResponse(*request);
		in_progress_.back().second = new_response;
		return CreateInstructionsForNewResopnse(*new_response);
	}

	IResponse *ResponseHolder::CreateNewResponse(const IRequest &request)
	{
		IResponse                *new_response = NULL;
		const std::string        &host         = GetHost(request);
		const conf::ServerConf   &vs_conf      = config_[host];
		const conf::LocationConf &location     = vs_conf.FindMatchingLocationConf(request.Path());
		bool                      is_cgi       = !location.GetCgiPath().empty();
		try {
			if (is_cgi) {
				new_response = NULL;
			} else {
				new_response = new StaticResponse(request, location);
			}
		} catch (http::HttpException &e) {
			delete new_response;
			new_response = new ErrorResponse(request.GetErrStatusCode(), vs_conf);
		}
		return new_response;
	}

	event::Instructions ResponseHolder::CreateInstructionsForNewResopnse(const IResponse &response)
	{
		event::Instructions insts;

		if (response.HasFd()) { // TODO cgiはread | write
			insts.push_back(event::Instruction(
				event::Instruction::kRegister, response.GetFd().Value(), event::Event::kRead
			));
		}
		if (response.HasReadyData()) {
			insts.push_back(event::Instruction(
				event::Instruction::kAppendEventType, conn_fd_, event::Event::kWrite
			));
		}
		return insts;
	}

	event::Instructions ResponseHolder::Perform(const event::Event &event)
	{
		IResponse *response = in_progress_.front().second;
		response->Perform(event);
	}

	Result<event::Instructions> ResponseHolder::Send()
	{
		event::Instructions insts;

		IResponse   *response    = in_progress_.front().second;
		Result<void> send_result = response->Send(conn_fd_);
		if (send_result.IsErr()) { // TODO conn_fdのwriteを外すべきか？
			insts.push_back(
				event::Instruction(event::Instruction::kUnregister, response->GetFd().Value())
			);
			delete response;
			request_del_(in_progress_.front().first);
			in_progress_.pop_front();
			return Result<event::Instructions>(insts, send_result.Err());
		}
		if (!response->HasReadyData()) {
			insts.push_back(event::Instruction(
				event::Instruction::kTrimEventType, conn_fd_, event::Event::kWrite
			));
		}
	}

	std::size_t ResponseHolder::size()
	{
		return in_progress_.size();
	}

} // namespace server
