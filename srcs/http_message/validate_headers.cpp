#include "validate_headers.hpp"

#include "error.hpp"
#include "webserv_utils.hpp"

namespace http_headers
{
	bool HasSingleHost(const FieldLines &field_lines)
	{
		(void)field_lines;
		return true;
	}

	bool IsValidTransferEncoding(const FieldLines &field_lines)
	{
		(void)field_lines;
		return true;
	}

	bool IsValidContentLength(const std::string &value)
	{
		if (value.empty()) {
			return true;
		}
		const bool has_single_value = value.find(",") == std::string::npos;
		if (!has_single_value) {
			return false;
		}
		if (value.empty()) {
			return false;
		}
		const bool start_with_digit = std::isdigit(value.at(0));
		return start_with_digit && utils::StrToLong(value).IsOk();
	}

	bool IsValidFieldLines(const FieldLines &field_lines)
	{
		HasSingleHost(field_lines);
		// bool has_content_length    = field_lines.Contains("content-length");
		// bool has_transfer_encoding = field_lines.Contains("transfer-encoding");
		// if (has_content_length && has_transfer_encoding) {
		// 	throw Error("400"); // TODO 例外だったりboolだったり微妙だから後で考える
		// } else if (has_content_length && !IsValidContentLength(field_lines)) {
		// 	return false;
		// } else if (has_transfer_encoding && !IsValidTransferEncoding(field_lines)) {
		// 	return false;
		// }
		return true;
	}

} // namespace http_headers
