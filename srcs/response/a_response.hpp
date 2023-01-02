#ifndef A_RESPONSE_HPP
#define A_RESPONSE_HPP

#include "event.hpp"
#include "i_request.hpp"
#include "meta_data_storage.hpp"
#include "queuing_reader.hpp"
#include "queuing_writer.hpp"

namespace response
{
	class AResponse : public q_buffer::QueuingReader,
					  public q_buffer::QueuingWriter,
					  public server::MetaDataStorage
	{
	  protected:
		const server::IRequest &request_;
		bool                    is_finished_;
		bool                    need_to_close_;

	  private:
		AResponse();
		AResponse(const AResponse &other);
		AResponse &operator=(const AResponse &rhs);

	  public:
		AResponse(const server::IRequest &request);
		virtual ~AResponse();
		virtual void           Perform(const event::Event &event) = 0;
		virtual bool           HasFd() const                      = 0;
		virtual Emptiable<int> GetFd() const                      = 0;
		virtual Result<void>   Send(int fd);
		bool                   HasReadyData() const;
		bool                   IsFinished() const;
		bool                   NeedToClose() const;
		std::size_t            size() const;
	};
} // namespace response
#endif
