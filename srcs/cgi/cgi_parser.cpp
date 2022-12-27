#include "cgi_parser.hpp"
#include "debug.hpp"
#include "http_define.hpp"
#include "http_exceptions.hpp"
#include "validate_field_line.hpp"
#include "webserv_utils.hpp"

namespace cgi
{
	const std::string CgiParser::kCr           = "\r";
	const std::string CgiParser::kLf           = "\n";
	const std::size_t CgiParser::kMaxLineSize  = 8192;
	const std::size_t CgiParser::kMaxTotalSize = 40000;

	CgiParser::CgiParser() : StatefulParser()
	{
		InitParseContext();
	}

	CgiParser::CgiParser(const CgiParser &other) : StatefulParser()
	{
		ctx_.fields = new http::FieldSection();
		*this       = other;
	}

	CgiParser::~CgiParser()
	{
		delete ctx_.fields;
	}

	CgiParser &CgiParser::operator=(const CgiParser &rhs)
	{
		if (&rhs == this) {
			return *this;
		}
		StatefulParser::operator=(rhs);
		*ctx_.fields      = *rhs.ctx_.fields;
		ctx_.state        = rhs.ctx_.state;
		ctx_.total_length = rhs.ctx_.total_length;
		return *this;
	}

	void CgiParser::InitParseContext()
	{
		ClearLoadedBytes();
		ctx_.fields       = new http::FieldSection();
		ctx_.state        = kStandby;
		ctx_.total_length = 0;
	}

	void CgiParser::DestroyParseContext()
	{
		utils::DeleteSafe(ctx_.fields);
		InitParseContext();
	}

	Emptiable<http::FieldSection *> CgiParser::Parse(q_buffer::QueuingBuffer &recieved)
	{
		if (recieved.empty()) {
			return Emptiable<http::FieldSection *>();
		}
		ctx_.state = kParsing;
		try {
			if (CreateFieldSection(recieved) == kDone) {
				log("field parse success", ctx_.fields);
				http::FieldSection *fields = ctx_.fields;
				InitParseContext();
				return fields;
			}
			return Emptiable<http::FieldSection *>();
		} catch (http::HttpException &e) {
			DestroyParseContext();
			throw e;
		}
	}

	server::StatefulParser::ParseResult
	CgiParser::CreateFieldSection(q_buffer::QueuingBuffer &recieved)
	{
		while (!recieved.empty()) {
			switch (LoadBytesWithDelim(recieved, kLf, kMaxLineSize)) {
			case kOverMaxSize:
				throw http::BadRequestException();
			case kNonParsable:
				continue;
			case kParsable:
				EraseLfOrCrLf(loaded_bytes_);
				if (loaded_bytes_.empty()) {
					log("field secton done. total len: ", ctx_.total_length);
					return kDone;
				}
				if (ctx_.total_length + loaded_bytes_.size() > kMaxTotalSize) {
					throw http::BadRequestException();
				}
				ctx_.fields->Append(FieldLine(loaded_bytes_));
				ctx_.total_length += loaded_bytes_.size();
				ClearLoadedBytes();
				continue;
			default:
				DBG_INFO;
				throw std::logic_error("chunked parser logic error");
			}
		}
		return kInProgress;
	}

	void CgiParser::EraseLfOrCrLf(std::string &field_line)
	{
		if (!utils::EndWith(field_line, kLf)) {
			return;
		}
		field_line.erase(field_line.rbegin());
		if (utils::EndWith(field_line, kCr)) {
			field_line.erase(field_line.rbegin());
		}
	}

	void CgiParser::DestroyFieldSection(const http::FieldSection *fields)
	{
		delete fields;
	}

	http::FieldSection *CgiParser::CopyFieldSection(const http::FieldSection *src)
	{
		if (src == NULL) {
			return NULL;
		}
		return new http::FieldSection(*src);
	}

} // namespace cgi
