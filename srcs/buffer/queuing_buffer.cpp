#include "queuing_buffer.hpp"

namespace q_buffer
{
	QueuingBuffer::QueuingBuffer() : buf_(), front_idx_(), size_() {}

	QueuingBuffer::QueuingBuffer(const QueuingBuffer &other)
	{
		*this = other;
	}

	Emptiable<char> QueuingBuffer::PopChar()
	{
		if (empty()) {
			return Emptiable<char>();
		}
		ByteArray &bytes = buf_.front();
		char       c     = bytes.at(front_idx_); // 例外は起きないはず
		front_idx_++;
		if (front_idx_ == bytes.size()) {
			front_idx_ = 0;
			buf_.pop_front();
		}
		size_--;
		return c;
	}

	std::vector<char> QueuingBuffer::PopAll()
	{
		if (buf_.empty()) {
			return std::vector<char>();
		}
		ByteArray &front = buf_.front();
		ByteArray  all(front.begin() + front_idx_, front.end());
		front_idx_ = 0;
		buf_.pop_front();
		for (; !buf_.empty();) {
			ByteArray &buf = buf_.front();
			std::copy(buf.begin(), buf.end(), std::back_inserter(all));
			buf_.pop_front();
		}
		size_ = 0;
		return all;
	}

	bool QueuingBuffer::empty() const
	{
		return buf_.empty();
	}

	QueuingBuffer &QueuingBuffer::operator=(const QueuingBuffer &other)
	{
		if (this == &other) {
			return *this;
		}
		buf_       = other.buf_;
		front_idx_ = other.front_idx_;
		size_      = other.size_;
		return *this;
	}

	std::size_t QueuingBuffer::size() const
	{
		return size_;
	}

	void QueuingBuffer::push_front(char c)
	{
		size_++;
		if (front_idx_ > 0) {
			front_idx_--;
			buf_.front()[front_idx_] = c;
			return;
		}
		buf_.push_front(ByteArray());
		buf_.front().push_back(c);
	}

} // namespace q_buffer
