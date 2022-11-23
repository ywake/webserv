#include "requset_parser.hpp"
#include "http_define.hpp"
#include "webserv_utils.hpp"

namespace server
{
	RequestParser::RequestParser() : state_(kStandBy) {}

	RequestParser::ErrStatus RequestParser::Parse(buffer::Buffer &recieved)
	{
		switch (state_) {
		case kStandBy:
			state_ = kStartLine;
			/* Falls through. */
		case kStartLine:
			return ParseStartLine(recieved);
		case kHeader:
			return ParseHeaderSection(recieved);
		case kBody:
			return ParseBody(recieved);
		}
		return ErrStatus();
	}

	RequestParser::ErrStatus RequestParser::ParseStartLine(buffer::Buffer &recieved)
	{
		(void)recieved;
		return ErrStatus();
	}

	RequestParser::ErrStatus RequestParser::ParseHeaderSection(buffer::Buffer &recieved)
	{
		(void)recieved;
		return ErrStatus();
	}

	RequestParser::ErrStatus RequestParser::ParseBody(buffer::Buffer &recieved)
	{
		(void)recieved;
		return ErrStatus();
	}
} // namespace server
