#include "buffer.hpp"
#include "http_define.hpp"

namespace buffer
{
	Buffer::Buffer(std::size_t max_inner_buf_size)
		: buf_(), idx_(), max_inner_buf_size_(max_inner_buf_size)
	{}

	Buffer::Buffer(const Buffer &other)
	{
		*this = other;
	}

	Result<void> Buffer::push_back(const std::vector<char> &data)
	{
		if (IsFull()) {
			return Error("buffer is full");
		}
		if (data.empty()) {
			return Result<void>();
		}
		buf_.push_back(data);
		return Result<void>();
	}

	Emptiable<char> Buffer::GetChar()
	{
		if (empty()) {
			return Emptiable<char>();
		}
		ByteArray &bytes = buf_.front();
		char       c     = bytes.at(idx_); // 例外は起きないはず
		idx_++;
		if (idx_ == bytes.size()) {
			PopFront();
		}
		return c;
	}

	std::vector<char> Buffer::GetAll()
	{
		if (buf_.empty()) {
			return std::vector<char>();
		}
		ByteArray &front = buf_.front();
		ByteArray  all(front.begin() + idx_, front.end());
		PopFront();
		for (; !buf_.empty();) {
			ByteArray &buf = buf_.front();
			std::copy(buf.begin(), buf.end(), std::back_inserter(all));
			PopFront();
		}
		return all;
	}

	bool Buffer::empty() const
	{
		return buf_.empty();
	}

	bool Buffer::IsFull() const
	{
		return buf_.size() == max_inner_buf_size_;
	}

	Buffer &Buffer::operator=(const Buffer &other)
	{
		if (this == &other) {
			return *this;
		}
		buf_                = other.buf_;
		idx_                = other.idx_;
		max_inner_buf_size_ = other.max_inner_buf_size_;
		return *this;
	}

	void Buffer::PopFront()
	{
		if (buf_.empty()) {
			return;
		}
		idx_ = 0;
		buf_.pop_front();
	}

	MessageBuffer::MessageBuffer(std::size_t max_innfer_buf_size = kDefaultMaxSize)
		: buf_(), max_inner_buf_size_(max_innfer_buf_size)
	{}

	MessageBuffer::MessageBuffer(const MessageBuffer &other)
	{
		*this = other;
	}

	Result<void> MessageBuffer::push_back(const http::ResponseMessage &data)
	{
		if (IsFull()) {
			return Error("buffer is full");
		}
		buf_.push_back(data);
		return Result<void>();
	}

	bool MessageBuffer::empty() const
	{
		return buf_.empty();
	}

	bool MessageBuffer::IsFull() const
	{
		return buf_.size() == max_inner_buf_size_;
	}

	MessageBuffer &MessageBuffer::operator=(const MessageBuffer &other)
	{
		if (this == &other) {
			return *this;
		}
		buf_                = other.buf_;
		max_inner_buf_size_ = other.max_inner_buf_size_;
		return *this;
	}

	LineBuffer::LineBuffer(std::size_t max_inner_buf_size = kDefaultMaxSize)
		: Buffer(max_inner_buf_size)
	{}

	LineBuffer::LineBuffer(const LineBuffer &other)
	{
		*this = other;
	}

	LineBuffer &LineBuffer::operator=(const LineBuffer &other)
	{
		if (this == &other) {
			return *this;
		}
		buf_                = other.buf_;
		idx_                = other.idx_;
		max_inner_buf_size_ = other.max_inner_buf_size_;
		leftover_           = other.leftover_;
		return *this;
	}

	Emptiable<std::string> LineBuffer::GetLine()
	{
		if (leftover_.empty() && empty()) {
			return leftover_;
		}
		std::string buf = leftover_;
		while (true) {
			Emptiable<char> res = GetChar();
			if (res.empty()) {
				break;
			}
			char c = res.Value();
			buf += c;
			if (c == http::kNl[0]) {
				leftover_.clear();
				return buf;
			}
		}
		leftover_ += buf;
		return Emptiable<std::string>();
	}
} // namespace buffer
