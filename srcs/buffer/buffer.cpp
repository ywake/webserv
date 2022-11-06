#include "buffer.hpp"

namespace buffer
{
	Buffer::Buffer(std::size_t max_inner_buf_size)
		: inner_buf_(), idx_(), max_inner_buf_size_(max_inner_buf_size)
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
		inner_buf_.push_back(data);
		return Result<void>();
	}

	Result<char> Buffer::GetChar()
	{
		if (empty()) {
			return Error("buffer is empty");
		}
		ByteArray &bytes = inner_buf_.front();
		char       c     = bytes.at(idx_); // 例外は起きないはず
		idx_++;
		if (idx_ == bytes.size()) {
			PopFront();
		}
		return c;
	}

	std::vector<char> Buffer::GetAll()
	{
		ByteArray all;

		for (; !inner_buf_.empty();) {
			ByteArray &buf = inner_buf_.front();
			std::copy(buf.begin(), buf.end(), std::back_inserter(all));
			inner_buf_.pop_front();
		}
		return all;
	}

	bool Buffer::empty() const
	{
		return inner_buf_.empty();
	}

	bool Buffer::IsFull() const
	{
		return inner_buf_.size() == max_inner_buf_size_;
	}

	Buffer &Buffer::operator=(const Buffer &other)
	{
		if (this == &other) {
			return *this;
		}
		inner_buf_          = other.inner_buf_;
		idx_                = other.idx_;
		max_inner_buf_size_ = other.max_inner_buf_size_;
		return *this;
	}

	void Buffer::PopFront()
	{
		if (inner_buf_.empty()) {
			return;
		}
		idx_ = 0;
		inner_buf_.pop_front();
	}

} // namespace buffer
