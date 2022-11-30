#ifndef STATIC_RESPONSE_HPP
#define STATIC_RESPONSE_HPP

#include "i_response.hpp"
#include "instruction.hpp"
#include "managed_fd.hpp"

namespace server
{
	class StaticResponse : public IResponse
	{
	  public:
		ManagedFd fd_;

	  public:
		event::Instructions Perform();
	};
} // namespace server

#endif
