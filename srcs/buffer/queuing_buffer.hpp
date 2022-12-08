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
		Emptiable<char>   PopChar();
		std::vector<char> PopAll();
		bool              empty() const;
		QueuingBuffer    &operator=(const QueuingBuffer &other);
		std::size_t       size() const;

		template <typename T>
		void push_back(const T &data)
		{
			if (data.empty()) {
				return;
			}
			buf_.push_back(ByteArray(data.begin(), data.end()));
			size_ += data.size();
		}

		/*
		o ... empty
		x ... filled

		buf	[o o x x]<-[x x x x]
				 |
				 front_idx_ == 2
		==============================
		input [x x x x x]
					 | |
		buf			[o o x x]<-[x x x x]
						 |
						 front_idx_ == 2
		==============================
		input [x x x]x x
					 | |
		buf          [x x x x]<-[x x x x]
					  |
					  front_idx_ == 0
		==============================
		input [x x x]
			   | | |
		buf   [     ]<-[x x x x]<-[x x x x]
						|
						front_idx_ == 0
		==============================
		input []

		buf   [x x x]<-[x x x x]<-[x x x x]
			   |
			   front_idx_ == 0
		*/
		template <typename T>
		void push_front(const T &data)
		{
			if (data.empty()) {
				return;
			}
			size_ += data.size();
			ByteArray  &front = buf_.front();
			std::size_t size  = data.size();
			for (; front_idx_ > 0 && size > 0; size--, front_idx_--) {
				front[front_idx_ - 1] = data[size - 1];
			}
			if (size == 0) {
				return;
			}
			buf_.push_front(ByteArray(data.begin(), data.begin() + size));
		}
	};
} // namespace q_buffer

#endif
