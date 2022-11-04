#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include "poll_instruction.hpp"
#include "result.hpp"
#include <deque>

namespace http
{
	class IResource
	{
	  protected:
		static const size_t           kCharacterQueueLength = 10;
		static const size_t           kMessageQueueLength   = 10;
		std::deque<std::vector<char>> character_q;
		std::deque<CgiResponse>       message_q;

	  public:
		typedef io_multiplexer::PollInstructions PollInstructions;
		virtual const Result<PollInstructions>   Send()    = 0;
		virtual const Result<PollInstructions>   Receive() = 0;
	};

} // namespace http

#endif // RESOURCE_HPP
