#include "response_holder.hpp"
#include "error_response.hpp"
#include "http_exceptions.hpp"
#include "static_response.hpp"

namespace server
{
	using namespace event;

	const std::size_t ResponseHolder::kMaxBufSize = 8196;

	inline const std::string &GetHost(const IRequest &request)
	{
		return request.Headers()["host"].front().GetValue();
	}

	Instructions ResponseHolder::StartNewResponse(IRequest *request)
	{
		in_progress_.push_back(ReqRes(request, NULL));
		if (request->IsErr()) {
			in_progress_.back().second =
				new ErrorResponse(request->GetErrStatusCode(), config_[GetHost(*request)]);
			return CreateInstructionsForNewResopnse(*in_progress_.back().second);
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

	Instructions ResponseHolder::CreateInstructionsForNewResopnse(const IResponse &response)
	{
		Instructions insts;

		if (response.HasFd()) { // TODO cgiはread | write
			insts.push_back(
				Instruction(Instruction::kRegister, response.GetFd().Value(), Event::kRead)
			);
		}
		if (response.HasReadyData()) { // ほんとはfrontのときだけ
			insts.push_back(Instruction(Instruction::kAppendEventType, conn_fd_, Event::kWrite));
		}
		return insts;
	}

	Instructions ResponseHolder::Perform(const event::Event &event)
	{
		Instructions insts;

		IResponse *response = in_progress_.front().second; // ほんとはmap[fd][resreq]から探す
		try {
			response->Perform(event);
			if (response->size() > kMaxBufSize) {
				insts.push_back(Instruction(
					Instruction::kTrimEventType, response->GetFd().Value(), Event::kRead
				));
			}
			if (response->HasReadyData()) { // ほんとはfrontのときだけ
				insts.push_back(Instruction(Instruction::kAppendEventType, conn_fd_, Event::kWrite)
				);
			}
			if (response->IsFinished()) {
				insts.push_back(Instruction(
					Instruction::kTrimEventType,
					response->GetFd().Value(),
					Event::kRead | Event::kWrite
				));
			}
			return insts;
		} catch (http::HttpException &e) {
			IRequest *request = in_progress_.front().first;
			delete response;
			in_progress_.back().second =
				new ErrorResponse(request->GetErrStatusCode(), config_[GetHost(*request)]);
			return CreateInstructionsForNewResopnse(*in_progress_.back().second);
		} // TODO local redir
	}

	Result<Instructions> ResponseHolder::Send()
	{
		Instructions insts;

		IResponse   *response    = in_progress_.front().second;
		Result<void> send_result = response->Send(conn_fd_);
		if (send_result.IsErr()) {
			is_fatal_ = true;
			return send_result.Err();
		}
		insts.push_back(
			Instruction(Instruction::kAppendEventType, response->GetFd().Value(), Event::kRead)
		);
		if (!response->HasReadyData()) {
			insts.push_back(Instruction(Instruction::kTrimEventType, conn_fd_, Event::kWrite));
		}
		if (!response->HasReadyData() && response->IsFinished()) {
			IRequest *request = in_progress_.front().first;
			is_fatal_         = request->IsFatal();
			Instructions i    = FinishFrontResponse();
			insts.splice(insts.end(), i);
			// ここでqueueに残ってれば開始したいけど今max queue size() 1だからやってない
		}
		return insts;
	}

	std::size_t ResponseHolder::size()
	{
		return in_progress_.size();
	}

	Instructions ResponseHolder::FinishFrontResponse()
	{
		Instructions insts;

		if (in_progress_.empty()) {
			return insts;
		}
		IRequest  *request  = in_progress_.front().first;
		IResponse *response = in_progress_.front().second;
		if (response->HasFd()) {
			insts.push_back(Instruction(Instruction::kUnregister, response->GetFd().Value()));
		}
		request_del_(request);
		delete (response);
		in_progress_.pop_front();
		return insts;
	}

	Instructions ResponseHolder::UnregisterAll()
	{
		Instructions insts;

		for (; !in_progress_.empty();) {
			Instructions i = FinishFrontResponse();
			insts.splice(insts.end(), i);
		}
		return insts;
	}

	bool ResponseHolder::IsFatal()
	{
		return is_fatal_;
	}

	ResponseHolder::~ResponseHolder()
	{
		for (; !in_progress_.empty();) {
			IRequest  *request  = in_progress_.front().first;
			IResponse *response = in_progress_.front().second;
			request_del_(request);
			delete (response);
			in_progress_.pop_front();
		}
	}

} // namespace server
