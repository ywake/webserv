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
	class QueuingBuffer
	{
	  protected:
		typedef std::vector<char>     ByteArray;
		typedef std::deque<ByteArray> InnerBuffer;

	  protected:
		InnerBuffer buf_;
		std::size_t idx_;

	  public:
		QueuingBuffer();
		QueuingBuffer(const QueuingBuffer &other);
		void              push_back(const std::vector<char> &data);
		Emptiable<char>   PopChar();
		std::vector<char> PopAll();
		bool              empty() const;
		QueuingBuffer    &operator=(const QueuingBuffer &other);
		std::size_t       size();

	  protected:
		void PopFront();
	};
} // namespace buffer

#endif
