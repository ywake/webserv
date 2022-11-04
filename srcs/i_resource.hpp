#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include "poll_instruction.hpp"

namespace http
{
	class IResource
	{
	  public:
		virtual const io_multiplexer::PollInstructions Send()    = 0;
		virtual const io_multiplexer::PollInstructions Receive() = 0;
	};

} // namespace http

#endif // RESOURCE_HPP
