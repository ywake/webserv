#ifndef RECIEVER_HPP
#define RECIEVER_HPP

#include "instruction.hpp"
#include "queuing_buffer.hpp"
#include "result.hpp"

namespace server
{
	class Reciever : public buffer::QueuingBuffer
	{
	  private:
		static const std::size_t kDefaultBufferSize;

	  private:
		int    fd_;
		size_t buffer_size_;
		bool   is_eof_;

	  public:
		Reciever(int fd = -1, std::size_t buffer_size = kDefaultBufferSize);
		Result<void> Recv();
		bool         IsEof();
	};
} // namespace server

#endif
