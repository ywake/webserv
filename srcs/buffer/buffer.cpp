#include "buffer.hpp"

namespace buffer
{
	Buffer::Buffer() : buf_(), idx_() {}

	Buffer::Buffer(const Buffer &other)
	{
		*this = other;
	}

	void Buffer::push_back(const std::vector<char> &data)
	{
		if (data.empty()) {
			return;
		}
		buf_.push_back(data);
	}

	Emptiable<char> Buffer::PopChar()
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

	std::vector<char> Buffer::PopAll()
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

	Buffer &Buffer::operator=(const Buffer &other)
	{
		if (this == &other) {
			return *this;
		}
		buf_ = other.buf_;
		idx_ = other.idx_;
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

	std::size_t Buffer::size()
	{
		return buf_.size();
	}

} // namespace buffer
