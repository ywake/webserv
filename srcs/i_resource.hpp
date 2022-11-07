#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include "poll_instruction.hpp"
#include "result.hpp"
#include <deque>

namespace http
{
	class IResource
	{
	  public:
		typedef io_multiplexer::PollInstructions PollInstructions;
		virtual const Result<PollInstructions>   Send()    = 0;
		virtual const Result<PollInstructions>   Receive() = 0;
	};

} // namespace http

#endif // RESOURCE_HPP
