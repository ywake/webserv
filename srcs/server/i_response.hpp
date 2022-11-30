#ifndef I_RESPONSE_HPP
#define I_RESPONSE_HPP

#include "instruction.hpp"
#include "result.hpp"

namespace server
{
	class IResponse
	{
	  public:
		virtual ~IResponse() {}

		virtual int                 GetFd() const = 0;
		virtual event::Instructions Perform()     = 0;
		virtual Result<void>        Send(int fd)  = 0;
	};
} // namespace server

#endif
