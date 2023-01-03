#include "a_response.hpp"

namespace response
{
	AResponse::AResponse(const server::IRequest &request)
		: request_(request), is_finished_(false), need_to_close_(false)
	{}

	Result<void> AResponse::Send(int fd)
	{
		return Write(fd);
	}

	bool AResponse::HasReadyData() const
	{
		return !QueuingBuffer::empty();
	}

	bool AResponse::IsFinished() const
	{
		return is_finished_;
	}

	bool AResponse::NeedToClose() const
	{
		return request_.NeedToClose() || need_to_close_;
	}

	std::size_t AResponse::size() const
	{
		return q_buffer::QueuingBuffer::size();
	}

	AResponse::~AResponse() {}

} // namespace response
