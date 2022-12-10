#include "http_define.hpp"

namespace http
{
	const std::string kCrLf             = "\r\n";
	const std::string kEmptyLine        = "\r\n\r\n";
	const std::string kWhiteSpaces      = " \t";
	const std::string kSp               = " ";
	const std::string kHttpVersion      = "HTTP/1.1";
	const std::string kConnection       = "connection";
	const std::string kContentLength    = "content-length";
	const std::string kTransferEncoding = "transfer-encoding";
	const std::string kChunked          = "chunked";
} // namespace http
