#include "validate_headers.hpp"
#include "validate_field_line.hpp"

#include "http_define.hpp"
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

		bool HasSingleHost(const HeaderSection::Values &values)
		{
			return values.size() == 1;
		}

		bool IsImplementedTransferCoding(const HeaderSection::Values &values)
		{
			for (HeaderSection::Values::const_iterator it = values.begin(); it != values.end();
				 ++it) {
				if (it->GetValue() != kChunked) {
					return false;
				}
			}
			return true;
		}

		bool IsValidTransferEncoding(const HeaderSection::Values &values)
		{
			if (!values.empty() && values.back().GetValue() != kChunked) {
				return false;
			}
			size_t chunked_count = 0;
			for (HeaderSection::Values::const_iterator it = values.begin(); it != values.end();
				 ++it) {
				if (it->GetValue() == kChunked) {
					chunked_count++;
				}
				if (chunked_count > 1) {
					return false;
				}
			}
			return true;
		}

		bool IsValidContentLength(const std::string &value)
		{
			if (value.empty()) {
				return false;
			}
			const bool start_with_digit = std::isdigit(value.at(0));
			return start_with_digit && utils::StrToLong(value).IsOk();
		}

		/**
		 * @brief
		 *
		 * @throw http::BadRequestException / http::NotImplementedException
		 */
		void ValidateHeaderSection(const HeaderSection &field_lines)
		{
			if (!HasSingleHost(field_lines["host"])) {
				throw BadRequestException();
			}
			bool has_content_length    = field_lines.Contains(kContentLength);
			bool has_transfer_encoding = field_lines.Contains(kTransferEncoding);
			if (has_content_length && has_transfer_encoding) {
				throw BadRequestException();
			} else if (has_transfer_encoding) {
				if (!IsValidTransferEncoding(field_lines[kTransferEncoding])) {
					throw BadRequestException();
				}
				if (!IsImplementedTransferCoding(field_lines[kTransferEncoding])) {
					throw NotImplementedException();
				}
			}
		}

	} // namespace headers
} // namespace http
