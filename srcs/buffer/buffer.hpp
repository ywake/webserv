#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <deque>
#include <vector>

#include "emptiable.hpp"
#include "response_message.hpp"
#include "result.hpp"

// TODO rename
namespace buffer
{
	// TODO rename
	class Buffer
	{
	  private:
		typedef std::vector<char>     ByteArray;
		typedef std::deque<ByteArray> InnerBuffer;

	  private:
		static const std::size_t kDefaultMaxSize = 10;
		InnerBuffer              buf_;
		std::size_t              idx_;
		std::size_t              max_inner_buf_size_;

	  public:
		Buffer(std::size_t max_inner_buf_size = kDefaultMaxSize);
		Buffer(const Buffer &other);
		Result<void>      push_back(const std::vector<char> &data);
		Emptiable<char>   GetChar();
		std::vector<char> GetAll();
		bool              empty() const;
		bool              IsFull() const;
		Buffer           &operator=(const Buffer &other);

	  private:
		void PopFront();
	};

	class MessageBuffer
	{
	  private:
		typedef std::deque<http::ResponseMessage> InnerBuffer;

	  private:
		static const std::size_t kDefaultMaxSize = 10;
		InnerBuffer              buf_;
		std::size_t              max_inner_buf_size_;

	  public:
		MessageBuffer(std::size_t max_innfer_buf_size = kDefaultMaxSize);
		MessageBuffer(const MessageBuffer &other);
		Result<void>   push_back(const http::ResponseMessage &data);
		bool           empty() const;
		bool           IsFull() const;
		MessageBuffer &operator=(const MessageBuffer &other);
	};
} // namespace buffer

#endif
