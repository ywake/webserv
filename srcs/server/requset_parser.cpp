#include "requset_parser.hpp"
#include "http_define.hpp"
#include "http_exceptions.hpp"
#include "webserv_utils.hpp"

namespace server
{
	RequestParser::RequestParser() : state_(kStandBy) {}

	RequestParser::ErrStatus RequestParser::Parse(buffer::Buffer &recieved)
	{
		try {
			switch (state_) {
			case kStandBy:
				state_ = kStartLine;
				/* Falls through. */
			case kStartLine:
				ParseStartLine(recieved);
				break;
			case kHeader:
				ParseHeaderSection(recieved);
				break;
			case kBody:
				ParseBody(recieved);
				break;
			}
			return ErrStatus();
		} catch (http::HttpException &e) {
			request_queue_.push_back(Error());
			// TODO delete
			state_ = kStandBy;
			return ErrStatus(e.GetStatusCode(), Error());
		}
	}

	void RequestParser::ParseStartLine(buffer::Buffer &recieved)
	{
		(void)recieved;
		return ErrStatus();
	}

	void RequestParser::ParseHeaderSection(buffer::Buffer &recieved)
	{
		(void)recieved;
	}

	void RequestParser::ParseBody(buffer::Buffer &recieved)
	{
		(void)recieved;
	}
} // namespace server
