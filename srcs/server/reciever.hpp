#ifndef RECIEVER_HPP
#define RECIEVER_HPP

#include "buffer.hpp"
#include "instruction.hpp"
#include "result.hpp"

namespace server
{
	class Reciever : public buffer::Buffer
	{
	  private:
		int    fd_;
		size_t buffer_size_;
		bool   is_eof_;

	  public:
		Reciever(int fd = -1, std::size_t buffer_size = 1024);
		Result<void> Recv();
		bool         IsEof();
	};
} // namespace server

#endif
