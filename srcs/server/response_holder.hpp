#ifndef RESPONSE_HOLDER_HPP
#define RESPONSE_HOLDER_HPP

#include <deque>
#include <netinet/in.h>
#include <utility>

#include "i_request.hpp"
#include "i_response.hpp"
#include "instruction.hpp"
#include "result.hpp"
#include "status_code.hpp"
#include "virtual_server_confs.hpp"

// パイプラインに必要な機能は未実装。max queue sizeは1でしか使えない
namespace server
{
	class ResponseHolder
	{
	  private:
		typedef void (*RequestDelFunc)(IRequest *&);
		typedef struct Task {
			IRequest   *request;
			IResponse  *response;
			std::size_t local_redir_count;
		} Task;

	  private:
		static const std::size_t kMaxBufSize;

		int                             conn_fd_;
		const struct sockaddr_storage  *server_;
		const struct sockaddr_storage  *client_;
		const conf::VirtualServerConfs *config_;
		std::deque<Task>                in_progress_;
		RequestDelFunc                  request_del_;
		bool                            need_to_close_;

	  private:
		ResponseHolder();
		ResponseHolder(const ResponseHolder &other);
		ResponseHolder &operator=(const ResponseHolder &rhs);

	  public:
		ResponseHolder(
			int                             conn_fd,
			const conf::VirtualServerConfs *conf,
			const struct sockaddr_storage  *server,
			const struct sockaddr_storage  *client,
			RequestDelFunc                  del
		);
		~ResponseHolder();
		event::Instructions         StartNewResponse(IRequest *request);
		event::Instructions         Perform(const event::Event &event);
		Result<event::Instructions> Send();
		std::size_t                 size();
		event::Instructions         UnregisterAll();
		bool                        NeedToClose();

	  private:
		event::Instructions FinishFrontResponse();
		event::Instructions AddNewResponse(Task *task);
		event::Instructions AddNewRedirectResponse(Task *task, const conf::LocationConf &location);
		event::Instructions AddNewCgiResponse(Task *task, const conf::LocationConf &location);
		event::Instructions AddNewStaticResponse(Task *task, const conf::LocationConf &location);
		event::Instructions AddNewErrorResponse(
			Task *task, const http::StatusCode &status_code, const conf::ServerConf &sv_conf
		);
		event::Instructions            CreateInstructionsForError(const IResponse &response);
		inline const conf::ServerConf &GetServerConf(const IRequest &request);
	};
} // namespace server

#endif
