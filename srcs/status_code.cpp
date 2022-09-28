#include "status_code.hpp"

namespace http
{
	StatusCode::StatusCode(Code code) : code_(code) {}

	StatusCode::StatusCode(const EmptyStatusCode &code) : code_(kUndefinedCode)
	{
		(void)code;
	}

	bool StatusCode::empty()
	{
		return code_ == kUndefinedCode;
	}

} // namespace http
