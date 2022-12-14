#include "field_parser.hpp"
#include "debug.hpp"
#include "http_define.hpp"
#include "http_exceptions.hpp"
#include "validate_field_line.hpp"
#include "webserv_utils.hpp"

namespace server
{

	const std::size_t FieldParser::kMaxLineSize  = 8192;
	const std::size_t FieldParser::kMaxTotalSize = 40000;

	FieldParser::FieldParser() : StatefulParser()
	{
		InitParseContext();
	}

	FieldParser::FieldParser(const FieldParser &other) : StatefulParser()
	{
		ctx_.fields = new http::FieldSection();
		*this       = other;
	}

	FieldParser::~FieldParser()
	{
		delete ctx_.fields;
	}

	FieldParser &FieldParser::operator=(const FieldParser &rhs)
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

	void FieldParser::InitParseContext()
	{
		ClearLoadedBytes();
		ctx_.fields       = new http::FieldSection();
		ctx_.state        = kStandby;
		ctx_.total_length = 0;
	}

	void FieldParser::DestroyParseContext()
	{
		utils::DeleteSafe(ctx_.fields);
		InitParseContext();
	}

	Emptiable<http::FieldSection *> FieldParser::Parse(q_buffer::QueuingBuffer &recieved)
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

	StatefulParser::ParseResult FieldParser::CreateFieldSection(q_buffer::QueuingBuffer &recieved)
	{
		while (!recieved.empty()) {
			switch (LoadFieldLine(recieved, kMaxLineSize)) {
			case kOverMaxSize:
				throw http::BadRequestException();
			case kNonParsable:
				continue;
			case kParsable:
				loaded_bytes_.erase(loaded_bytes_.size() - http::kCrLf.size());
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

	StatefulParser::LoadResult
	FieldParser::LoadFieldLine(q_buffer::QueuingBuffer &recieved, std::size_t max_bytes)
	{
		for (;;) {
			if (recieved.empty()) {
				return kNonParsable;
			}
			Emptiable<char> c = recieved.PopChar();
			if (utils::EndWith(loaded_bytes_, http::kCrLf) && !http::abnf::IsRws(c.Value())) {
				recieved.push_front(c.Value());
				return kParsable;
			}
			loaded_bytes_ += c.Value();
			if (loaded_bytes_.size() > max_bytes) {
				return kOverMaxSize;
			}
			if (loaded_bytes_ == http::kCrLf) {
				return kParsable;
			}
		}
	}

	void FieldParser::DestroyFieldSection(const http::FieldSection *fields)
	{
		delete fields;
	}

	http::FieldSection *FieldParser::CopyFieldSection(const http::FieldSection *src)
	{
		if (src == NULL) {
			return NULL;
		}
		return new http::FieldSection(*src);
	}

} // namespace server
