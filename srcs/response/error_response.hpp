#ifndef ERROR_RESPONSE_HPP
#define ERROR_RESPONSE_HPP

#include "i_request.hpp"
#include "i_response.hpp"
#include "managed_fd.hpp"
#include "meta_data_storage.hpp"
#include "queuing_reader.hpp"
#include "queuing_writer.hpp"
#include "server_conf.hpp"
#include "status_code.hpp"

namespace response
{
	class ErrorResponse : public server::IResponse,
						  public q_buffer::QueuingReader,
						  public q_buffer::QueuingWriter,
						  public server::MetaDataStorage
	{
	  private:
		const server::IRequest &request_;
		const conf::ServerConf &config_;
		ManagedFd               managed_fd_;
		bool                    is_finished_;

	  public:
		ErrorResponse(
			const server::IRequest &request,
			const http::StatusCode &status_code,
			const conf::ServerConf &conf
		);
		void           Perform(const event::Event &event);
		Result<void>   Send(int fd);
		bool           HasReadyData() const;
		bool           HasFd() const;
		Emptiable<int> GetFd() const;
		bool           IsFinished() const;
		std::size_t    size() const;

	  private:
		void PushDefaultErrorPage(const http::StatusCode &code);
	};
} // namespace response

#endif
