#include "validate_headers.hpp"

#include "error.hpp"

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
	bool IsValidContentLength(const FieldLines &field_lines)
	{
		(void)field_lines;
		return true;
	}

	bool IsValidFieldLines(const FieldLines &field_lines)
	{
		HasSingleHost(field_lines);
		bool has_content_length    = field_lines.Contains("content-length");
		bool has_transfer_encoding = field_lines.Contains("transfer-encoding");
		if (has_content_length && has_transfer_encoding) {
			throw Error("400"); // TODO 例外だったりboolだったり微妙だから後で考える
		} else if (has_content_length) {
			IsValidContentLength(field_lines);
		} else if (has_transfer_encoding) {
			IsValidTransferEncoding(field_lines);
		}
		return true;
	}

} // namespace http_headers
