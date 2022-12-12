#ifndef BODY_WRITER_HPP
#define BODY_WRITER_HPP

#include <vector>

#include "result.hpp"

namespace server
{
	class BodyWriter
	{
	  private:
		static const std::size_t kDefaultChunkSize;
		const std::vector<char> *body_;
		std::size_t              chunk_size_;
		std::size_t              used_size_;

	  public:
		BodyWriter(
			const std::vector<char> *body = NULL, std::size_t chunk_size_ = kDefaultChunkSize
		);
		BodyWriter(const BodyWriter &other);
		BodyWriter  &operator=(const BodyWriter &rhs);
		Result<void> Write(int fd);
		bool         IsFinished() const;
	};
} // namespace server

#endif
