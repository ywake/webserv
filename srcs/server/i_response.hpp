#ifndef I_RESPONSE_HPP
#define I_RESPONSE_HPP

#include "instruction.hpp"
#include "result.hpp"

namespace server
{
	class IResponse
	{
	  public:
		int                         GetFd() const = 0;
		event::Instructions         Perform()     = 0;
		Result<event::Instructions> Send()        = 0;
	};
} // namespace server

#endif
