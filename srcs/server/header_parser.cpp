#include "header_parser.hpp"
#include "host_port.hpp"
#include "http_define.hpp"
#include "http_exceptions.hpp"
#include "parse_http_utils.hpp"
#include "validate_headers.hpp"
#include "validate_http_char.hpp"
#include "webserv_utils.hpp"

namespace server
{
	HeaderParser::HeaderParser() : FieldParser() {}

	HeaderParser::HeaderParser(const FieldParser &other) : FieldParser()
	{
		*this = other;
	}

	FieldParser &HeaderParser::operator=(const FieldParser &rhs)
	{
		if (&rhs == this) {
			return *this;
		}
		FieldParser::operator=(rhs);
		return *this;
	}

	Emptiable<http::FieldSection *> HeaderParser::Parse(q_buffer::QueuingBuffer &recieved)
	{
		Emptiable<http::FieldSection *> headers_or_empty = FieldParser::Parse(recieved);
		if (headers_or_empty.empty()) {
			return Emptiable<http::FieldSection *>();
		}
		http::FieldSection &headers = *headers_or_empty.Value();
		ValidateHost(headers["host"]);
		if (headers.Contains(http::kContentLength)) {
			ValidateContentLength(headers[http::kContentLength]);
		}
		if (headers.Contains(http::kTransferEncoding)) {
			http::FieldSection::Values &te_header = headers[http::kTransferEncoding];
			te_header                             = ParseTransferEncoding(te_header);
			if (!http::headers::IsValidTransferEncoding(te_header) ||
				!http::headers::IsImplementedTransferCoding(te_header)) {
				throw http::NotImplementedException();
			}
		}
		if (headers.Contains(http::kConnection)) {
			http::FieldSection::Values &con_header = headers[http::kConnection];
			con_header                             = ParseConnection(con_header);
		}
		return &headers;
	}

	void HeaderParser::ValidateHost(const http::FieldSection::Values &values)
	{
		if (!http::headers::HasSingleHost(values)) {
			throw http::BadRequestException();
		}
		(void)http::abnf::HostPort(values.front().GetValue());
	}

	// [RFC 9110 8.6 末尾] 複数ある場合は却下してもよい
	void HeaderParser::ValidateContentLength(const http::FieldSection::Values &values)
	{
		if (values.empty()) {
			return;
		}
		if (values.size() != 1 || http::headers::IsValidContentLength(values.front().GetValue())) {
			throw http::BadRequestException();
		}
	}

	http::FieldSection::Values
	HeaderParser::ParseTransferEncoding(const http::FieldSection::Values &values)
	{
		HeaderSection::Values new_values;

		for (HeaderSection::Values::const_iterator itr = values.begin(); itr != values.end();
			 ++itr) {
			const std::string      &value = itr->GetValue();
			std::vector<ThinString> list  = http::ParseList(value);
			for (std::vector<ThinString>::const_iterator it = list.begin(); it != list.end();
				 ++it) {
				if (!it->empty() && !http::abnf::IsToken(*it)) {
					throw http::NotImplementedException();
				}
				// valueも大文字小文字を無視
				new_values.push_back(HeaderValue(utils::ToLowerString(it->ToString())));
			}
		}
		return new_values;
	}

	// Connection        = #connection-option
	// connection-option = token
	// TODO "close"が含まれていたら応答後に接続を閉じる [MUST]
	http::FieldSection::Values
	HeaderParser::ParseConnection(const http::FieldSection::Values &values)
	{
		HeaderSection::Values new_values;

		for (HeaderSection::Values::const_iterator itr = values.begin(); itr != values.end();
			 ++itr) {
			const std::string      &value = itr->GetValue();
			std::vector<ThinString> list  = http::ParseList(value);
			for (std::vector<ThinString>::const_iterator it = list.begin(); it != list.end();
				 ++it) {
				if (!it->empty() && !http::abnf::IsToken(*it)) {
					throw http::BadRequestException();
				}
				// valueも大文字小文字を無視
				new_values.push_back(HeaderValue(utils::ToLowerString(it->ToString())));
			}
		}
		return new_values;
	}

} // namespace server
