#include "connection.hpp"

namespace server
{
	Connection::RequestParser::RequestParser() : state_(kStandBy) {}

	Connection::RequestParser::ErrStatus Connection::RequestParser::Parse()
	{
		switch (state_) {
		case kStandBy:
			state_ = kStartLine;
			/* Falls through. */
		case kStartLine:
			return ParseStartLine();
		case kHeader:
			return ParseHeaderSection();
		case kBody:
			return ParseBody();
		}
		return ErrStatus();
	}

	Connection::RequestParser::ErrStatus Connection::RequestParser::ParseStartLine()
	{
		return ErrStatus();
	}

	Connection::RequestParser::ErrStatus Connection::RequestParser::ParseHeaderSection()
	{
		return ErrStatus();
	}

	Connection::RequestParser::ErrStatus Connection::RequestParser::ParseBody()
	{
		return ErrStatus();
	}
} // namespace server
