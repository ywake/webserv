#ifndef STATIC_RESPONSE_HPP
#define STATIC_RESPONSE_HPP

#include "i_request.hpp"
#include "i_response.hpp"
#include "location_conf.hpp"
#include "managed_fd.hpp"
#include "queuing_reader.hpp"
#include "queuing_writer.hpp"

namespace server
{
	class StaticResponse : public IResponse,
						   public q_buffer::QueuingReader,
						   public q_buffer::QueuingWriter
	{
	  private:
		const IRequest           &request_;
		const conf::LocationConf &config_;
		ManagedFd                 managed_fd_;
		bool                      is_finished_;

	  public:
		StaticResponse(const IRequest &request, const conf::LocationConf &conf);
		void           Perform(const event::Event &event);
		Result<void>   Send(int fd);
		bool           HasReadyData() const;
		bool           HasFd() const;
		Emptiable<int> GetFd() const;
		bool           IsFinished() const;
		std::size_t    size() const;

	  private:
		void InitGetMethod();
	};
} // namespace server

#endif
