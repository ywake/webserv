#include "response_holder.hpp"
#include "cgi_response.hpp"
#include "debug.hpp"
#include "delete_method.hpp"
#include "error_response.hpp"
#include "get_method.hpp"
#include "http_define.hpp"
#include "http_exceptions.hpp"
#include "local_redirect_exception.hpp"
#include "post_method.hpp"
#include "redirect.hpp"

namespace cgi
{
	static const std::size_t kMaxLocalRedirects = 10;
}

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
		: conn_fd_(-1),
		  client_(),
		  config_(NULL),
		  in_progress_(),
		  request_del_(NULL),
		  need_to_close_(false)
	{}

	ResponseHolder::ResponseHolder(
		int                             conn_fd,
		const conf::VirtualServerConfs *conf,
		const struct sockaddr_storage  *server,
		const struct sockaddr_storage  *client,
		RequestDelFunc                  del
	)
		: conn_fd_(conn_fd),
		  server_(server),
		  client_(client),
		  config_(conf),
		  in_progress_(),
		  request_del_(del),
		  need_to_close_(false)
	{
		if (config_ == NULL || server_ == NULL || client_ == NULL || del == NULL) {
			DBG_INFO;
			throw std::logic_error("ResponseHolder logic error");
		}
	}

	Instructions ResponseHolder::StartNewResponse(IRequest *request)
	{
		Instructions insts;
		Task         task = {request, NULL, 0};

		if (request->IsErr()) {
			task.response = new response::ErrorResponse(
				*request, task.request->GetErrStatusCode(), GetServerConf(*task.request)
			);
			insts = CreateInitialInstructions(*task.response);
		} else {
			insts = AddNewResponse(&task);
		}
		in_progress_.push_back(task);
		return insts;
	}

	Instructions ResponseHolder::AddNewResponse(Task *task)
	{
		IRequest                 &request     = *task->request;
		const conf::ServerConf   &sv_conf     = GetServerConf(request);
		const conf::LocationConf &location    = sv_conf.FindMatchingLocationConf(request.Path());
		bool                      is_redirect = !location.GetRedirect().empty();
		bool                      is_cgi      = !location.GetCgiPath().empty();
		try {
			if (task->local_redir_count > cgi::kMaxLocalRedirects) {
				throw http::InternalServerErrorException();
			} else if (!location.IsAllowedMethod(request.Method())) {
				throw http::MethodNotAllowedException();
			} else if (is_redirect) {
				task->response = new response::Redirect(*task->request, location);
			} else if (is_cgi) {
				task->response = new cgi::CgiResponse(*task->request, location, server_, client_);
			} else if (request.Method() == http::methods::kGet) {
				task->response = new response::GetMethod(*task->request, location);
			} else if (request.Method() == http::methods::kPost) {
				task->response = new response::PostMethod(*task->request, location);
			} else if (request.Method() == http::methods::kDelete) {
				task->response = new response::DeleteMethod(*task->request, location);
			} else {
				DBG_INFO;
				throw std::logic_error("invalid method");
			}
		} catch (http::HttpException &e) {
			delete task->response;
			task->response =
				new response::ErrorResponse(*task->request, e.GetStatusCode(), sv_conf);
		}
		return CreateInitialInstructions(*task->response);
	}

	Instructions ResponseHolder::CreateInitialInstructions(const response::AResponse &response)
	{
		Instructions insts;

		if (response.HasFd()) {
			insts.push_back(Instruction(
				Instruction::kRegister, response.GetFd().Value(), Event::kRead | Event::kWrite
			));
		}
		if (response.HasReadyData()) { // ほんとはfrontのときだけ
			insts.push_back(Instruction(Instruction::kAppendEventType, conn_fd_, Event::kWrite));
		}
		return insts;
	}

	Instructions ResponseHolder::Perform(const event::Event &event)
	{
		Instructions insts;

		Task                &task = in_progress_.front(); // ほんとはmap[fd][task]から探す
		IRequest            *request  = task.request;
		response::AResponse *response = task.response; // ほんとはmap[fd][task]から探す
		try {
			response::AResponse::FinEventType fin = response->Perform(event);
			if (response->size() > kMaxBufSize) {
				insts.push_back(Instruction(
					Instruction::kTrimEventType, response->GetFd().Value(), Event::kRead
				));
			}
			if (fin != event::Event::kEmpty) {
				insts.push_back(
					Instruction(Instruction::kTrimEventType, response->GetFd().Value(), fin)
				);
			}
			if (response->HasReadyData()) { // ほんとはfrontのときだけ
				insts.push_back(Instruction(Instruction::kAppendEventType, conn_fd_, Event::kWrite)
				);
			}
			if (response->IsFinished()) {
				insts.push_back(Instruction(Instruction::kUnregister, response->GetFd().Value()));
			}
			return insts;
		} catch (cgi::LocalRedirectException &e) {
			log("local redirect");
			insts = Instructions();
			insts.push_back(Instruction(Instruction::kUnregister, response->GetFd().Value()));
			utils::DeleteSafe(task.response);
			request->SetPath(e.Path());
			request->SetQuery(e.Query());
			task.local_redir_count++;
			Instructions add_insts = AddNewResponse(&task);
			insts.splice(insts.end(), add_insts);
			return insts;
		} catch (http::HttpException &e) {
			insts = Instructions();
			insts.push_back(Instruction(Instruction::kUnregister, response->GetFd().Value()));
			delete response;
			task.response =
				new response::ErrorResponse(*request, e.GetStatusCode(), GetServerConf(*request));
			Instructions i = CreateInitialInstructions(*in_progress_.front().response);
			insts.splice(insts.end(), i);
			return insts;
		}
	}

	Result<Instructions> ResponseHolder::Send()
	{
		Instructions insts;

		response::AResponse *response    = in_progress_.front().response;
		Result<void>         send_result = response->Send(conn_fd_);
		if (send_result.IsErr()) {
			need_to_close_ = true;
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
			log("finish", response->GetFd().Value());
			need_to_close_ = response->NeedToClose();
			PopFrontResponse();
			insts.push_back(Instruction(Instruction::kRegister, conn_fd_, Event::kRead));
			// ここでqueueに残ってれば開始したいけど今max queue size() 1だからやってない
		}
		return insts;
	}

	std::size_t ResponseHolder::size()
	{
		return in_progress_.size();
	}

	void ResponseHolder::PopFrontResponse()
	{
		if (in_progress_.empty()) {
			return;
		}
		IRequest            *request  = in_progress_.front().request;
		response::AResponse *response = in_progress_.front().response;
		request_del_(request);
		delete (response);
		in_progress_.pop_front();
	}

	Instructions ResponseHolder::UnregisterAll()
	{
		Instructions insts;

		for (; !in_progress_.empty();) {
			response::AResponse *response = in_progress_.front().response;
			if (response->HasFd()) {
				insts.push_back(Instruction(Instruction::kUnregister, response->GetFd().Value()));
			}
			PopFrontResponse();
		}
		return insts;
	}

	bool ResponseHolder::NeedToClose()
	{
		return need_to_close_;
	}

	ResponseHolder::~ResponseHolder()
	{
		for (; !in_progress_.empty();) {
			IRequest            *request  = in_progress_.front().request;
			response::AResponse *response = in_progress_.front().response;
			request_del_(request);
			delete (response);
			in_progress_.pop_front();
		}
	}

} // namespace server
