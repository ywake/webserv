#include "buffer.hpp"

namespace buffer
{
	QueuingBuffer::QueuingBuffer() : buf_(), idx_() {}

	QueuingBuffer::QueuingBuffer(const QueuingBuffer &other)
	{
		*this = other;
	}

	void QueuingBuffer::push_back(const std::vector<char> &data)
	{
		if (data.empty()) {
			return;
		}
		buf_.push_back(data);
	}

	Emptiable<char> QueuingBuffer::PopChar()
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

	std::vector<char> QueuingBuffer::PopAll()
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

	bool QueuingBuffer::empty() const
	{
		return buf_.empty();
	}

	QueuingBuffer &QueuingBuffer::operator=(const QueuingBuffer &other)
	{
		if (this == &other) {
			return *this;
		}
		buf_ = other.buf_;
		idx_ = other.idx_;
		return *this;
	}

	void QueuingBuffer::PopFront()
	{
		if (buf_.empty()) {
			return;
		}
		idx_ = 0;
		buf_.pop_front();
	}

	std::size_t QueuingBuffer::size()
	{
		return buf_.size();
	}

} // namespace buffer
