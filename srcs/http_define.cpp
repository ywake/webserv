#include "http_define.hpp"

namespace http
{
	const std::string kCrLf        = "\r\n";
	const std::string kEmptyLine   = "\r\n\r\n";
	const std::string kWhiteSpaces = " \t";
	const std::string kSp          = " ";
	const std::string kHttpVersion = "HTTP/1.1";
	namespace methods
	{
		const std::string kGet    = "GET";
		const std::string kDelete = "DELETE";
		const std::string kPost   = "POST";
	} // namespace methods

} // namespace http
