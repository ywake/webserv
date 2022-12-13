#include "bytes_loader.hpp"
#include "debug.hpp"
#include "http_define.hpp"
#include "http_exceptions.hpp"
#include "parse_chunked.hpp"
#include "webserv_utils.hpp"

namespace server
{
	BytesLoader::BytesLoader(std::size_t max_bytes) : max_bytes_(max_bytes)
	{
		InitParseContext();
	}

	BytesLoader::BytesLoader(const BytesLoader &other)
	{
		*this = other;
	}

	BytesLoader::~BytesLoader()
	{
		delete ctx_.bytes;
	}

	BytesLoader &BytesLoader::operator=(const BytesLoader &rhs)
	{
		if (&rhs == this) {
			return *this;
		}
		max_bytes_  = rhs.max_bytes_;
		*ctx_.bytes = *rhs.ctx_.bytes;
		ctx_.state  = rhs.ctx_.state;
		return *this;
	}

	Emptiable<std::vector<char> *> BytesLoader::Parse(q_buffer::QueuingBuffer &recieved)
	{
		ctx_.state = kParsing;
		for (; ctx_.bytes->size() < max_bytes_ && !recieved.empty();) {
			Emptiable<char> c = recieved.PopChar();
			ctx_.bytes->push_back(c.Value());
		}
		if (ctx_.bytes->size() < max_bytes_) {
			return Emptiable<std::vector<char> *>();
		}
		std::vector<char> *bytes = ctx_.bytes;
		InitParseContext();
		return bytes;
	}

	bool BytesLoader::HasInCompleteData()
	{
		return ctx_.state != kStandby;
	}

	void BytesLoader::InitParseContext()
	{
		ctx_.bytes = new std::vector<char>();
		ctx_.state = kStandby;
	}

	void BytesLoader::DestroyParseContext()
	{
		utils::DeleteSafe(ctx_.bytes);
		InitParseContext();
	}

} // namespace server
