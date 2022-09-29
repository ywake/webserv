#include "validate_headers.hpp"
#include "validate_field_line.hpp"

#include "http_exceptions.hpp"
#include "webserv_utils.hpp"

static const std::string kCrLf = "\r\n";

namespace http_headers
{
	bool HasObsFold(const ThinString &str)
	{
		for (ThinString remained = str;;) {
			std::size_t crlf_pos = remained.find(kCrLf);
			if (crlf_pos == ThinString::npos) {
				return false;
			}
			remained = remained.substr(crlf_pos);
			if (http_abnf::StartWithObsFold(remained)) {
				return true;
			}
		}
	}

	bool HasSingleHost(const HeaderSection &field_lines)
	{
		(void)field_lines;
		return true;
	}

	bool IsValidTransferEncoding(const HeaderSection &field_lines)
	{
		(void)field_lines;
		return true;
	}

	bool IsValidContentLength(const std::string &value)
	{
		if (value.empty()) {
			return false;
		}
		const bool has_single_value = value.find(",") == std::string::npos;
		if (!has_single_value || HasObsFold(value)) {
			return false;
		}
		const bool start_with_digit = std::isdigit(value.at(0));
		return start_with_digit && utils::StrToLong(value).IsOk();
	}

	bool IsValidFieldLines(const HeaderSection &field_lines)
	{
		HasSingleHost(field_lines);
		// bool has_content_length    = field_lines.Contains("content-length");
		// bool has_transfer_encoding = field_lines.Contains("transfer-encoding");
		// if (has_content_length && has_transfer_encoding) {
		// 	throw BadRequestException(); // TODO 例外だったりboolだったり微妙だから後で考える
		// } else if (has_content_length && !IsValidContentLength(field_lines)) {
		// 	return false;
		// } else if (has_transfer_encoding && !IsValidTransferEncoding(field_lines)) {
		// 	return false;
		// }
		return true;
	}

} // namespace http_headers
