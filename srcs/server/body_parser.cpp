#include "body_parser.hpp"
#include "debug.hpp"
#include "http_define.hpp"
#include "http_exceptions.hpp"
#include "webserv_utils.hpp"

namespace server
{
	// TODO あとでconfとかに動かしたい
	const conf::ServerConf &
	GetServerConf(const conf::VirtualServerConfs &config, const http::FieldSection::Values &hosts)
	{
		if (hosts.empty()) {
			return config.GetDefaultServerConf();
		} else {
			const std::string &host = hosts.front().GetValue();
			return config[host];
		}
	}

	BodyParser::BodyParser(const conf::VirtualServerConfs *v_server_confs)
		: max_body_size_(), v_server_confs_(v_server_confs)
	{
		if (v_server_confs_ == NULL) {
			DBG_INFO;
			throw std::logic_error("body parser construct logic error");
		}
		InitParseContext();
	}

	BodyParser::BodyParser(const BodyParser &other)
	{
		*this = other;
	}

	BodyParser &BodyParser::operator=(const BodyParser &rhs)
	{
		if (&rhs == this) {
			return *this;
		}
		max_body_size_  = rhs.max_body_size_;
		v_server_confs_ = rhs.v_server_confs_;
		ctx_            = rhs.ctx_;
		return *this;
	}

	Emptiable<std::vector<char> *>
	BodyParser::Parse(q_buffer::QueuingBuffer &recieved, http::FieldSection &headers)
	{
		try {
			if (ctx_.state == kStandby) {
				ctx_.state = kParsing;
				InitMaxSize(headers);
				InitMode(headers);
			}
			Emptiable<std::vector<char> *> body;
			if (ctx_.mode == kContentLength) {
				body = ctx_.bytes_loader.Parse(recieved);
			} else if (ctx_.mode == kChunked) {
				body = ctx_.chunked_parser.Parse(recieved);
			} else {
				DBG_INFO;
				throw std::logic_error("body parser mode error");
			}
			if (body.empty()) {
				return Emptiable<std::vector<char> *>();
			}
			InitParseContext();
			return body.Value();
		} catch (http::HttpException &e) {
			DestroyParseContext();
			throw e;
		}
	}

	void BodyParser::InitMaxSize(const http::FieldSection &headers)
	{
		const conf::ServerConf &conf = GetServerConf(*v_server_confs_, headers["host"]);
		max_body_size_               = conf.GetClientMaxBodySize();
	}

	void BodyParser::InitMode(const http::FieldSection &headers)
	{
		// この時点では正しいvalueが来ることは保証されている
		if (headers.Contains(http::kTransferEncoding)) {
			ctx_.mode           = kChunked;
			ctx_.chunked_parser = ChunkedParser(max_body_size_);
		} else if (headers.Contains(http::kContentLength)) {
			ctx_.mode                      = kContentLength;
			const std::string &len_str     = headers[http::kContentLength].front().GetValue();
			std::size_t        content_len = utils::StrToUnsignedLong(len_str).Val();
			if (content_len > max_body_size_) {
				throw http::ContentTooLargeException();
			}
			ctx_.bytes_loader = BytesLoader(content_len);
		} else {
			DBG_INFO;
			throw std::logic_error("body parser logic error");
		}
	}

	bool BodyParser::HasInCompleteData()
	{
		return ctx_.state != kStandby;
	}

	void BodyParser::InitParseContext()
	{
		ctx_.bytes_loader   = BytesLoader();
		ctx_.chunked_parser = ChunkedParser();
		ctx_.state          = kStandby;
		ctx_.mode           = kUninitialized;
	}

	void BodyParser::DestroyParseContext()
	{
		InitParseContext();
	}

	void BodyParser::DestroyBody(const std::vector<char> *body)
	{
		delete body;
	}

	std::vector<char> *BodyParser::CopyBody(const std::vector<char> *src)
	{
		if (src == NULL) {
			return NULL;
		}
		return new std::vector<char>(*src);
	}
} // namespace server
