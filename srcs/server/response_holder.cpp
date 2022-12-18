#include "response_holder.hpp"
#include "debug.hpp"
#include "delete_method.hpp"
#include "error_response.hpp"
#include "get_method.hpp"
#include "http_define.hpp"
#include "http_exceptions.hpp"
#include "post_method.hpp"

namespace server
{
	using namespace event;

	const std::size_t ResponseHolder::kMaxBufSize = 8196;

	inline const conf::ServerConf &ResponseHolder::GetServerConf(const IRequest &request)
	{
		if (request.Host().empty()) {
			return config_->GetDefaultServerConf();
		} else {
			return (*config_)[request.Host()];
		}
	}

	ResponseHolder::ResponseHolder()
		: conn_fd_(-1), config_(NULL), in_progress_(), request_del_(NULL), is_fatal_(false)
	{}

	ResponseHolder::ResponseHolder(
		int conn_fd, const conf::VirtualServerConfs *conf, RequestDelFunc del
	)
		: conn_fd_(conn_fd), config_(conf), in_progress_(), request_del_(del), is_fatal_(false)
	{
		if (config_ == NULL || del == NULL) {
			DBG_INFO;
			throw std::logic_error("ResponseHolder logic error");
		}
	}

	Instructions ResponseHolder::StartNewResponse(IRequest *request)
	{
		Instructions insts;
		Task         task = {request, NULL};

		if (request->IsErr()) {
			task.response = new response::ErrorResponse(
				*request, task.request->GetErrStatusCode(), config_->GetDefaultServerConf()
			);
			insts = CreateInstructionsForError(*task.response);
		} else {
			insts = AddNewResponse(&task);
		}
		in_progress_.push_back(task);
		return insts;
	}

	Instructions ResponseHolder::AddNewResponse(Task *task)
	{
		const conf::ServerConf   &vs_conf = GetServerConf(*task->request);
		const conf::LocationConf &location =
			vs_conf.FindMatchingLocationConf(task->request->Path());
		bool is_cgi = !location.GetCgiPath().empty();
		try {
			if (is_cgi) {
				return AddNewCgiResponse(task, location);
			} else {
				return AddNewStaticResponse(task, location);
			}
		} catch (http::HttpException &e) {
			delete task->response;
			return AddNewErrorResponse(task, e.GetStatusCode(), vs_conf);
		}
	}

	Instructions ResponseHolder::AddNewCgiResponse(Task *task, const conf::LocationConf &location)
	{
		(void)location;
		Instructions insts;
		task->response = NULL;

		if (task->response->HasFd()) {
			insts.push_back(Instruction(
				Instruction::kRegister,
				task->response->GetFd().Value(),
				Event::kRead | Event::kWrite
			));
		}
		if (task->response->HasReadyData()) { // ほんとはfrontのときだけ
			insts.push_back(Instruction(Instruction::kAppendEventType, conn_fd_, Event::kWrite));
		}
		return insts;
	}

	Instructions
	ResponseHolder::AddNewStaticResponse(Task *task, const conf::LocationConf &location)
	{
		Instructions insts;

		// TODO 405 not allowed
		if (task->request->Method() == http::methods::kGet) {
			task->response = new response::GetMethod(*task->request, location);
			Instruction i =
				Instruction(Instruction::kRegister, task->response->GetFd().Value(), Event::kWrite);
			insts.push_back(i);
		} else if (task->request->Method() == http::methods::kPost) {
			task->response = new response::PostMethod(*task->request, location);
			Instruction i =
				Instruction(Instruction::kRegister, task->response->GetFd().Value(), Event::kRead);
			insts.push_back(i);
		} else if (task->request->Method() == http::methods::kDelete) {
			task->response = new response::DeleteMethod(*task->request, location);
		} else {
			DBG_INFO;
			throw std::logic_error("invalid method");
		}
		if (task->response->HasReadyData()) { // ほんとはfrontのときだけ
			insts.push_back(Instruction(Instruction::kAppendEventType, conn_fd_, Event::kWrite));
		}
		return insts;
	}

	Instructions ResponseHolder::AddNewErrorResponse(
		Task *task, const http::StatusCode &status_code, const conf::ServerConf &sv_conf
	)
	{
		task->response = new response::ErrorResponse(*task->request, status_code, sv_conf);
		return CreateInstructionsForError(*task->response);
	}

	Instructions ResponseHolder::CreateInstructionsForError(const IResponse &response)
	{
		Instructions insts;

		if (response.HasFd()) {
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

		Task      &task     = in_progress_.front(); // ほんとはmap[fd][task]から探す
		IRequest  *request  = task.request;
		IResponse *response = task.response; // ほんとはmap[fd][task]から探す
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
			// TODO unregister
			delete response;
			task.response =
				new response::ErrorResponse(*request, e.GetStatusCode(), GetServerConf(*request));
			return CreateInstructionsForError(*in_progress_.front().response);
		} // TODO local redir
	}

	Result<Instructions> ResponseHolder::Send()
	{
		Instructions insts;

		IResponse   *response    = in_progress_.front().response;
		Result<void> send_result = response->Send(conn_fd_);
		if (send_result.IsErr()) {
			is_fatal_ = true;
			return send_result.Err();
		}
		if (response->HasFd()) {
			insts.push_back(
				Instruction(Instruction::kAppendEventType, response->GetFd().Value(), Event::kRead)
			);
		}
		if (!response->HasReadyData()) {
			insts.push_back(Instruction(Instruction::kTrimEventType, conn_fd_, Event::kWrite));
		}
		if (!response->HasReadyData() && response->IsFinished()) {
			IRequest *request = in_progress_.front().request;
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
		IRequest  *request  = in_progress_.front().request;
		IResponse *response = in_progress_.front().response;
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
			IRequest  *request  = in_progress_.front().request;
			IResponse *response = in_progress_.front().response;
			request_del_(request);
			delete (response);
			in_progress_.pop_front();
		}
	}

} // namespace server
