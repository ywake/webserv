#include "cgi_response.hpp"

#include "debug.hpp"
#include "http_exceptions.hpp"
#include <sys/socket.h>

namespace cgi
{
	/**
	 * IResponse
	 */
	void CgiResponse::Perform(const event::Event &event)
	{
		if (event.event_type & event::Event::kHangUp) {
			log("cgi hangup");
			push_back(std::string("a"));
			is_hup_ = true;
			return;
		}
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
		if (parent_fd_.GetFd() == ManagedFd::kNofd || is_finished_) {
			return;
		}
		log("read from cgi");
		switch (state_) {
		case kHeader:
			OnHeader();
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
		if (cgi_receiver_.size() > kMaxLoadSize) { // TODO 定数
			return;
		}
		ssize_t read_size = cgi_receiver_.Read(parent_fd_.GetFd());
		log("cgi read size", read_size);
		if (read_size <= 0) {
			log(read_size == 0 ? "End of script output before headers" : "cgi read failed");
			throw http::InternalServerErrorException();
		}
		Emptiable<http::FieldSection *> fs = field_parser_.Parse(cgi_receiver_);
		// TODO FieldParserをCGI仕様に変更（obsfoldなし、NL＝LF）
		if (fs.empty()) {
			return;
		}
		state_ = kBody;
		// TODO レスポンスタイプごとに分岐
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
		q_buffer::QueuingBuffer::size();
		server::MetaDataStorage::PushWithCrLf();
		q_buffer::QueuingBuffer::push_back(cgi_receiver_.PopAll());
	}

	void CgiResponse::OnBody()
	{
		if (q_buffer::QueuingBuffer::size() > kMaxLoadSize) {
			return;
		}
		ssize_t read_size = this->Read(parent_fd_.GetFd());
		log("cgi body read size", read_size);
		if (read_size < 0) {
			throw http::InternalServerErrorException();
		}
		if (read_size == 0) {
			is_finished_ = true;
		}
	}

	Result<void> CgiResponse::Send(int fd)
	{
		// switch (state_) {
		// case kHeader:
		// 	break;
		// case kBody:
		// 	SendHeader();
		// default:
		// 	break;
		// }
		if (is_hup_) {
			log("write hup size", size());
			q_buffer::QueuingBuffer::operator=(q_buffer::QueuingBuffer());
			is_finished_ = true;
			return Result<void>();
		}
		log("cgi buf data before", size());
		Result<void> res = Write(fd);
		log("cgi buf data rest", size());
		return res;
		// return Write(fd);
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
