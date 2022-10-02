#include "validate_headers.hpp"
#include "validate_field_line.hpp"

#include "http_exceptions.hpp"
#include "webserv_utils.hpp"

static const std::string kCrLf = "\r\n";

namespace http
{
	namespace headers
	{
		bool HasObsFold(const ThinString &str)
		{
			for (ThinString remained = str;;) {
				std::size_t crlf_pos = remained.find(kCrLf);
				if (crlf_pos == ThinString::npos) {
					return false;
				}
				remained = remained.substr(crlf_pos);
				if (http::abnf::StartWithObsFold(remained)) {
					return true;
				}
			}
		}
		bool HasSingleHost(const HeaderSection &field_lines)
		{
			try {
				HeaderSection::Values values = field_lines.at("host");
				if (values.size() == 1) {
					return true;
				}
			} catch (const std::out_of_range &e) {
			}
			return false;
		}

		bool IsValidTransferEncoding(const HeaderSection &field_lines)
		{
			try {
				HeaderSection::Values values = field_lines.at("transfer-encoding");
				if (values.back().GetValue() != "chunked") {
					return false;
				}
				size_t chunked_count = 0;
				for (HeaderSection::Values::iterator it = values.begin(); it != values.end();
					 ++it) {
					if (it->GetValue() != "chunked") {
						throw http::NotImplementedException();
					} else {
						chunked_count++;
					}
					if (chunked_count > 1) {
						return false;
					}
				}
				return true;
			} catch (const std::out_of_range &e) {
				return true;
			}
		}

		bool IsValidContentLength(const std::string &value)
		{
			if (value.empty()) {
				return false;
			}
			const bool start_with_digit = std::isdigit(value.at(0));
			return start_with_digit && utils::StrToLong(value).IsOk();
		}

		// TODO 例外だったりboolだったり微妙だから後で考える
		// -> 400と501の区別が必要なので例外にする
		/**
		 * @brief
		 *
		 * @throw http::BadRequestException / http::NotImplementedException
		 */
		void ValidateHeaderSection(const HeaderSection &field_lines)
		{
			if (!HasSingleHost(field_lines)) {
				throw http::BadRequestException();
			}
			bool has_content_length    = field_lines.Contains("content-length");
			bool has_transfer_encoding = field_lines.Contains("transfer-encoding");
			if (has_content_length && has_transfer_encoding) {
				throw http::BadRequestException();
			} else if (has_transfer_encoding && !IsValidTransferEncoding(field_lines)) {
				throw http::BadRequestException();
			}
		}

	} // namespace headers
} // namespace http
