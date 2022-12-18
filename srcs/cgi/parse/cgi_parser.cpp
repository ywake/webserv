#include "cgi_parser.hpp"

/**

CGI-Response = docurment-response
			/ local-redir-response
			/ client-redir-response
			/ client-redirdoc-response

document-response = Content-Type [ Status ] *other-field CRLF response-body
local-redir-response = local-Location CRLF
client-redir-response = client-Location *extension-field CRLF
client-redirdoc-response = client-Location Status Content-Type *other-field CRLF
response-body

Content-Type = "Content-Type:" media-type CRLF

Location = local-Location | client-Location
	client-Location = "Location:" fragment-URI CRLF
		fragement-URI = absoluteURI ["#" fragment]
			fragment = *URI-char
	local-Location = "Location:" local-pathquery CRLF
		local-pathquery = abs-path ["?" query]
		abs-path = "/" path-segments
		path-segments = segment *( "/" segment )
			segment = *pchar
		pchar = unreserved | escaped | ":" | "@" | "&" | "=" | "+" | "$" | ","

Status = "Status:" status-code SP reason-phrase CRLF
	status-code = 3DIGIT
	reason-phrase = *TEXT

*/
namespace cgi
{

	Parser::Parser() : q_buffer::QueuingReader(), parse_end_(false) {}

	Parser::~Parser() {}

	ssize_t Parser::Read(int fd)
	{
		ssize_t size = q_buffer::QueuingReader::Read(fd);
		if (size < 0) {
			return size;
		}
		if (size == 0) {
			parse_end_ = true;
		}
		return size;
	}

	bool Parser::IsFinished() const
	{
		return parse_end_;
	}

	std::string Parser::ToString()
	{
		std::vector<char> buf = PopAll();
		return std::string(buf.begin(), buf.end());
	}

} // namespace cgi
