#include "cgi_response.hpp"

#include "debug.hpp"
#include "http_exceptions.hpp"

#include <sstream>
#include <sys/socket.h>

namespace cgi
{
	/**
	 * IResponse
	 */
	void CgiResponse::Perform(const event::Event &event)
	{
		if (event.event_type & event::Event::kWrite) {
			log("cgi write ready");
			OnWriteReady();
		}
		if (event.event_type & event::Event::kRead) {
			log("cgi read ready");
			OnReadReady();
		}
	}

	void CgiResponse::OnWriteReady()
	{
		if (parent_fd_.GetFd() == ManagedFd::kNofd) {
			log("nofd");
			return;
		}
		if (body_writer_.IsFinished()) {
			log("write to cgi is finished");
			if (shutdown(parent_fd_.GetFd(), SHUT_WR) < 0) {
				log("shutdown failed");
				throw http::InternalServerErrorException();
			}
			return;
		}
		log("write body to cgi");
		if (body_writer_.Write(parent_fd_.GetFd()).IsErr()) {
			shutdown(parent_fd_.GetFd(), SHUT_WR);
		}
	}

	void CgiResponse::OnReadReady()
	{
		log("read from cgi");
		if (!HasFd()) {
			throw std::logic_error("logci error: nofd perfrom in cgi");
		}
		if (cgi_receiver_.IsEof() || is_finished_) {
			log("cgi read fired on finished");
			return;
		}
		Result<void> res = cgi_receiver_.Recv();
		if (res.IsErr()) {
			log("cgi read failed");
			throw http::InternalServerErrorException();
		}
		switch (state_) {
		case kHeader:
			OnHeader();
			// eof処理をbodyの有無で分岐
			// bodyあるならfallthrough;
			break;
		case kBody:
			log("cgi body read");
			OnBody();
			break;
		default:
			throw std::logic_error("cgi read logic error");
			break;
		}

		// Parse();
		// parser_.Parse();
		// if (parser_.IsFinished()) {
		// 	q_buffer::QueuingBuffer::push_back(parser_.ToString());
		// }
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
		state_ = kBody;
		// TODO レスポンスタイプごとに分岐
		MetaDataStorage::StoreHeader(http::kServer, http::kServerName);
		if (!fs.Value()->Contains(http::kTransferEncoding)) { // TODO body必要なときのみ
			MetaDataStorage::StoreHeader(http::kTransferEncoding, http::kChunked);
		}
		http::FieldSection::FieldLines fl = fs.Value()->GetMap();
		for (http::FieldSection::FieldLines::iterator it = fl.begin(); it != fl.end(); ++it) {
			std::string                key   = it->first;
			http::FieldSection::Values value = it->second;
			// Serverは無視して上書き
			// ContentLengthは無視
			if (key == http::kServer || key == http::kContentLength) {
				continue;
			}
			// TransferEncodingは最終符号化法が"chunked"であればそのまま、ほかはchunkedを末尾に追加
			if (key == http::kTransferEncoding) {
				if (!value.empty() && value.back() == http::kChunked) {
					value.pop_back();
				}
				value.push_back(http::kChunked);
			}
			std::list<std::string> lst;
			for (http::FieldSection::Values::const_iterator it = value.begin(); it != value.end();
				 ++it) {
				lst.push_back(it->GetValue());
			}
			server::MetaDataStorage::StoreHeader(key, lst.begin(), lst.end());
		}
		server::MetaDataStorage::PushWithCrLf();
		CgiParser::DestroyFieldSection(fs.Value());
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
		if (cgi_receiver_.IsEof()) {
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
