#ifndef QUEUING_BUFFER_HPP
#define QUEUING_BUFFER_HPP

#include <deque>
#include <vector>

#include "emptiable.hpp"
#include "result.hpp"

namespace q_buffer
{
	class QueuingBuffer
	{
	  protected:
		typedef std::vector<char>     ByteArray;
		typedef std::deque<ByteArray> InnerBuffer;

	  protected:
		InnerBuffer buf_;
		std::size_t front_idx_;
		std::size_t size_;

	  public:
		QueuingBuffer();
		QueuingBuffer(const QueuingBuffer &other);
		void              push_back(const std::vector<char> &data);
		Emptiable<char>   PopChar();
		std::vector<char> PopAll();
		bool              empty() const;
		QueuingBuffer    &operator=(const QueuingBuffer &other);
		std::size_t       size() const;
	};
} // namespace q_buffer

#endif
