#include "queuing_buffer.hpp"

namespace q_buffer
{
	QueuingBuffer::QueuingBuffer() : buf_(), front_idx_() {}

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

	void QueuingBuffer::push_back(const std::string &data)
	{
		if (data.empty()) {
			return;
		}
		buf_.push_back(ByteArray(data.begin(), data.end()));
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
		return *this;
	}

	std::size_t QueuingBuffer::size()
	{
		return buf_.size();
	}

} // namespace q_buffer
