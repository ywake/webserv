#ifndef META_DATA_STORAGE_HPP
#define META_DATA_STORAGE_HPP

#include <sstream>

#include "http_define.hpp"
#include "queuing_buffer.hpp"
#include "status_code.hpp"
#include "webserv_utils.hpp"

namespace server
{
	class MetaDataStorage : public virtual q_buffer::QueuingBuffer
	{
	  private:
		std::string meta_data_;

	  public:
		MetaDataStorage();
		MetaDataStorage(const MetaDataStorage &other);
		MetaDataStorage &operator=(const MetaDataStorage &rhs);

		void StoreStatusLine(const std::string &version, const http::StatusCode &status_code);
		void StoreHeader(const std::string &name, const std::string &value);

		template <typename Iterator>
		void
		StoreHeader(const std::string &name, const Iterator &value_begin, const Iterator &value_end)
		{
			meta_data_ += name + ":";
			for (Iterator it = value_begin; it != value_end; ++it) {
				meta_data_ += *it + ", ";
			}
			if (utils::EndWith(meta_data_, ", ")) {
				meta_data_.erase(meta_data_.size() - 2);
			}
		}

		void Push();
		void PushWithCrLf();
	};
} // namespace server

#endif
