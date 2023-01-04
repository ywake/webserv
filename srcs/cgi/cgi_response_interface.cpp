#include <cerrno>
#include <cstdio>
#include <sstream>
#include <sys/socket.h>
#include <time.h>

#include "cgi_response.hpp"
#include "debug.hpp"
#include "http_exceptions.hpp"
#include "local_redirect_exception.hpp"
#include "parse_abnf_core_rules.hpp"
#include "request_target.hpp"

namespace
{
	bool IsTimeOut(const struct timespec &start, time_t lifetime_sec)
	{
		struct timespec now;
		if (clock_gettime(CLOCK_MONOTONIC_RAW, &now) == -1) {
			DBG_INFO;
			log("istimeout clock_gettime", strerror(errno));
		}
		log("time duration", now.tv_sec - start.tv_sec);
		return now.tv_sec - start.tv_sec >= lifetime_sec;
	}
} // namespace

namespace cgi
{
	/**
	 * IResponse
	 */
	response::AResponse::FinEventType CgiResponse::Perform(const event::Event &event)
	{
		FinEventType fin = event::Event::kEmpty;

		if (event.event_type & event::Event::kWrite) {
			log("cgi write ready");
			fin |= OnWriteReady();
		}
		if (event.event_type & event::Event::kRead) {
			log("cgi read ready");
			fin |= OnReadReady();
		}
		return fin;
	}

	response::AResponse::FinEventType CgiResponse::OnWriteReady()
	{
		if (body_writer_.IsFinished()) {
			log("write to cgi is finished");
			if (shutdown(parent_fd_.GetFd(), SHUT_WR) < 0) {
				log("shutdown failed");
				throw http::InternalServerErrorException();
			}
			return event::Event::kWrite;
		}
		log("write body to cgi");
		if (body_writer_.Write(parent_fd_.GetFd()).IsErr()) {
			if (shutdown(parent_fd_.GetFd(), SHUT_WR) < 0) {
				log("shutdown failed");
				throw http::InternalServerErrorException();
			}
			return event::Event::kWrite;
		}
		return event::Event::kEmpty;
	}

	response::AResponse::FinEventType CgiResponse::OnReadReady()
	{
		log("read from cgi");
		if (cgi_receiver_.IsEof() || is_finished_) {
			log("cgi read fired on finished");
			return event::Event::kRead;
		}
		Result<void> res = cgi_receiver_.Recv();
		if (res.IsErr()) {
			log("cgi read failed");
			throw http::InternalServerErrorException();
		}
		switch (state_) {
		case kHeader:
			OnHeader();
			if (state_ != kBody) {
				break;
			}
			/* fall through */
		case kBody:
			log("cgi body read");
			OnBody();
			break;
		default:
			throw std::logic_error("cgi read logic error");
			break;
		}
		return is_finished_ ? event::Event::kRead : event::Event::kEmpty;
	}

	void CgiResponse::OnHeader()
	{
		Emptiable<http::FieldSection *> fs = field_parser_.Parse(cgi_receiver_);
		if (fs.empty()) {
			if (cgi_receiver_.IsEof()) {
				log("End of script output before headers");
				throw http::InternalServerErrorException();
			}
			return;
		}
		const http::FieldSection &field_section = *fs.Value();
		if (field_section.Contains("content-type")) {
			if (PushMetaDataToSendBody(field_section).IsErr()) {
				CgiParser::DestroyFieldSection(fs.Value());
				throw http::InternalServerErrorException();
			}
			state_ = kBody;
		} else if (IsClientRedirect(field_section)) {
			PushMetaDataForClientRedirect(field_section["location"].front().GetValue());
			is_finished_ = true;
		} else if (IsLocalRedirect(field_section)) {
			ThrowLocalRedirect(field_section);
		} else {
			CgiParser::DestroyFieldSection(fs.Value());
			throw http::InternalServerErrorException();
		}
		CgiParser::DestroyFieldSection(fs.Value());
	}

	Result<void> CgiResponse::PushMetaDataToSendBody(const http::FieldSection &field_section)
	{
		http::StatusCode code;
		if (field_section.Contains("status")) {
			Result<http::StatusCode> status = ParseStatusCode(field_section["status"]);
			if (status.IsErr()) {
				return Error();
			}
			code = status.Val();
		} else {
			code = http::StatusCode(http::StatusCode::kOK);
		}
		need_to_close_ = code == http::StatusCode::kBadRequest;
		MetaDataStorage::StoreStatusLine(http::kHttpVersion, code);
		StoreHeadersToSendBody(field_section);
		MetaDataStorage::PushWithCrLf();
		return Result<void>();
	}

	void CgiResponse::PushMetaDataForClientRedirect(const std::string &uri)
	{
		MetaDataStorage::StoreStatusLine(http::kHttpVersion, http::StatusCode::kFound);
		MetaDataStorage::StoreHeader(http::kServer, http::kServerName);
		MetaDataStorage::StoreHeader("location", uri);
		MetaDataStorage::StoreHeader("Connection", NeedToClose() ? "close" : "keep-alive");
		MetaDataStorage::PushWithCrLf();
	}

	void CgiResponse::ThrowLocalRedirect(const http::FieldSection &field_section)
	{
		const std::string loc = field_section["location"].front().GetValue();
		CgiParser::DestroyFieldSection(&field_section);
		RequestTarget      tgt   = RequestTarget(OriginForm(loc));
		const std::string &path  = tgt.GetRequestFormData().path_;
		const std::string &query = tgt.GetRequestFormData().query_;
		throw LocalRedirectException(path, query);
	}

	// Status         = "Status:" status-code SP reason-phrase NUL
	// status-code    = "200" | "302" | "400" | "501" | extension-code
	// extension-code = 3digit
	// reason-phrase  = *TEXT
	Result<http::StatusCode> CgiResponse::ParseStatusCode(const http::FieldSection::Values &values)
	{
		if (values.size() != 1) {
			return Error();
		}
		const std::string &val = values.front().GetValue();
		if (val.size() < http::StatusCode::kCodeDigits) {
			return Error();
		}
		std::string former = val.substr(0, http::StatusCode::kCodeDigits);
		std::string latter = val.substr(http::StatusCode::kCodeDigits);
		if (!http::StatusCode::IsValidCode(former)) {
			return Error();
		}
		http::StatusCode::Code code = utils::StrToUnsignedLong(former).Val();
		if (latter.empty()) {
			return http::StatusCode(code);
		}
		if (latter[0] != ' ') {
			return Error();
		}
		std::string phrase = latter.substr(1);
		return http::StatusCode(code, phrase);
	}

	// [RFC 9112 10.2.2]
	// A Location field value cannot allow a list of members
	// because the comma list separator is a valid data character within a URI-reference.
	bool CgiResponse::IsClientRedirect(const http::FieldSection &field_section) const
	{
		if (field_section.GetMap().size() != 1) {
			return false;
		}
		const http::FieldSection::Values &values = field_section["location"];
		if (values.size() != 1) {
			return false;
		}
		const std::string &val = values.front().GetValue();
		return !val.empty() && val[0] != '/';
	}

	bool CgiResponse::IsLocalRedirect(const http::FieldSection &field_section) const
	{
		if (field_section.GetMap().size() != 1) {
			return false;
		}
		const http::FieldSection::Values &values = field_section["location"];
		if (values.size() != 1) {
			return false;
		}
		const std::string &val = values.front().GetValue();
		return !val.empty() && val[0] == '/';
	}

	void CgiResponse::StoreHeadersToSendBody(const http::FieldSection &field_section)
	{
		MetaDataStorage::StoreHeader(http::kServer, http::kServerName);
		MetaDataStorage::StoreHeader("Connection", NeedToClose() ? "close" : "keep-alive");
		if (!field_section.Contains(http::kTransferEncoding)) {
			MetaDataStorage::StoreHeader(http::kTransferEncoding, http::kChunked);
		}
		const http::FieldSection::FieldLines &fl = field_section.GetMap();
		for (http::FieldSection::FieldLines::const_iterator it = fl.begin(); it != fl.end(); ++it) {
			const std::string         &key    = it->first;
			http::FieldSection::Values values = it->second;
			// [RFC3875 6.3.4.]
			// SHOULD resolve any conflicts between header fields returned by the script ~
			// Serverは無視して上書き, ContentLengthは無視
			if (key == http::kServer || key == http::kContentLength || key == "status" ||
				key == http::kConnection) {
				continue;
			}
			// TransferEncodingは最終符号化法が"chunked"であればそのまま、ほかはchunkedを末尾に追加
			if (key == http::kTransferEncoding) {
				if (values.empty() || values.back() != http::kChunked) {
					values.push_back(http::kChunked);
				}
			}
			std::list<std::string> lst;
			for (http::FieldSection::Values::const_iterator it = values.begin(); it != values.end();
				 ++it) {
				lst.push_back(it->GetValue());
			}
			MetaDataStorage::StoreHeader(key, lst.begin(), lst.end());
		}
	}

	void CgiResponse::OnBody()
	{
		static const std::string kLastChunk = "0" + http::kCrLf + http::kCrLf;
		std::string              chunk;
		std::stringstream        ss;

		if (!cgi_receiver_.empty()) {
			ss << std::hex << cgi_receiver_.size();
			chunk += ss.str() + http::kCrLf;
			while (!cgi_receiver_.empty()) {
				Emptiable<char> c = cgi_receiver_.PopChar();
				chunk += c.Value();
			}
			chunk += http::kCrLf;
		}
		if (cgi_receiver_.IsEof() || IsTimeOut(time_, kLifeTimeSec)) {
			chunk += kLastChunk;
			is_finished_ = true;
		}
		push_back(chunk);
	}

	bool CgiResponse::HasFd() const
	{
		return parent_fd_.GetFd() != ManagedFd::kNofd;
	}

	Emptiable<int> CgiResponse::GetFd() const
	{
		return parent_fd_.GetFd();
	}
} // namespace cgi
