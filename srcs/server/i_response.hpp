#ifndef I_RESPONSE_HPP
#define I_RESPONSE_HPP

#include "emptiable.hpp"
#include "event.hpp"
#include "result.hpp"

namespace server
{
	class IResponse
	{
	  public:
		virtual ~IResponse() {}

		virtual void           Perform(const event::Event &event) = 0;
		virtual Result<void>   Send(int fd)                       = 0;
		virtual bool           HasReadyData() const               = 0;
		virtual bool           HasFd() const                      = 0;
		virtual Emptiable<int> GetFd() const                      = 0;
		virtual std::size_t    size() const                       = 0;
		virtual bool           IsFinished() const                 = 0;
	};
} // namespace server

#endif
