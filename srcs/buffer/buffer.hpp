#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <deque>
#include <vector>

#include "emptiable.hpp"
#include "result.hpp"

// TODO rename
namespace buffer
{
	// TODO rename
	class Buffer
	{
	  protected:
		typedef std::vector<char>     ByteArray;
		typedef std::deque<ByteArray> InnerBuffer;

	  protected:
		static const std::size_t kDefaultMaxSize = 10;
		InnerBuffer              buf_;
		std::size_t              idx_;
		std::size_t              max_inner_buf_size_;

	  public:
		Buffer(std::size_t max_inner_buf_size = kDefaultMaxSize);
		Buffer(const Buffer &other);
		Result<void>      push_back(const std::vector<char> &data);
		Emptiable<char>   PopChar();
		std::vector<char> PopAll();
		bool              empty() const;
		bool              IsFull() const;
		Buffer           &operator=(const Buffer &other);

	  private:
		void PopFront();
	};
} // namespace buffer

#endif
