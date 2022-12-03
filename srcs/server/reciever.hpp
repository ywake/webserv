#ifndef RECIEVER_HPP
#define RECIEVER_HPP

#include "instruction.hpp"
#include "queuing_reader.hpp"
#include "result.hpp"

namespace server
{
	class Reciever : public q_buffer::QueuingReader
	{
	  private:
		int    fd_;
		size_t buffer_size_;
		bool   is_eof_;

	  public:
		Reciever(int fd = -1, std::size_t recv_buf_size = QueuingReader::kDefaultReadBufSize);
		Result<void> Recv();
		bool         IsEof();
	};
} // namespace server

#endif
