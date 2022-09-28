#include "validate_headers.hpp"
#include "validate_field_line.hpp"

#include "host_port.hpp"
#include "result.hpp"
#include "status_code.hpp"
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
		// 	throw Error("400"); // TODO 例外だったりboolだったり微妙だから後で考える
		// } else if (has_content_length && !IsValidContentLength(field_lines)) {
		// 	return false;
		// } else if (has_transfer_encoding && !IsValidTransferEncoding(field_lines)) {
		// 	return false;
		// }
		return true;
	}

	bool IsValidHost(const std::string &host)
	{
		try {
			http::HostPort host_port(host);
			// find :
			return true;
		} catch (Error) {
			return false;
		}
	}

	http::StatusCode ValidateHost(const HeaderSection &field_lines)
	{
		if (!HasSignleHost() || !IsValidHost(host)) {
			return http::StatusCode::kBadRequest;
		}
		return http::EmptyStatusCode();
	}

	http::StatusCode ValidateContentLegnth(const std::string &value)
	{
		if (value.empty()) {
			return http::EmptyStatusCode();
		}
		const bool has_single_value = value.find(",") == std::string::npos;
		if (!has_single_value || HasObsFold(value)) {
			return http::StatusCode::kBadRequest;
		}
		const bool start_with_digit = std::isdigit(value.at(0));
		if (!start_with_digit || utils::StrToLong(value).IsErr()) {
			return http::StatusCode::kBadRequest;
		}
		return http::EmptyStatusCode();
	}

	http::StatusCode ValidateTransferEncoding(const std::string &value)
	{
		(void)value;
		return http::EmptyStatusCode();
	}

	http::StatusCode ValidateHeaderSection(const HeaderSection &field_lines)
	{
		http::StatusCode err_code;

		err_code = ValidateHost();
		if (!err_code.empty()) {
			return err_code;
		}
		err_code = ValidateContentLegnth();
		if (!err_code.empty()) {
			return err_code;
		}
		err_code = ValidateTransferEncoding();
		if (!err_code.empty()) {
			return err_code;
		}
		return http::EmptyStatusCode();
	}

} // namespace http_headers
