#include <sstream>

#include "meta_data_storage.hpp"

namespace server
{
	MetaDataStorage::MetaDataStorage() : QueuingBuffer() {}

	MetaDataStorage::MetaDataStorage(const MetaDataStorage &other) : QueuingBuffer()
	{
		*this = other;
	}

	MetaDataStorage &MetaDataStorage::operator=(const MetaDataStorage &rhs)
	{
		if (&rhs == this) {
			return *this;
		}
		QueuingBuffer::operator=(rhs);
		meta_data_ = rhs.meta_data_;
		return *this;
	}

	void MetaDataStorage::StoreStatusLine(
		const std::string &version, const http::StatusCode &status_code
	)
	{
		std::stringstream ss;

		ss << status_code.GetCode();
		meta_data_ += version + " " + ss.str() + " " + status_code.GetReasonPhrase() + http::kCrLf;
	}

	void MetaDataStorage::StoreHeader(const std::string &name, const std::string &value)
	{
		meta_data_ += name + ": " + value + http::kCrLf;
	}

	void MetaDataStorage::PushWithCrLf()
	{
		meta_data_ += http::kCrLf;
		Push();
	}

	void MetaDataStorage::Push()
	{
		QueuingBuffer::push_back(meta_data_);
		meta_data_ = std::string();
	}

} // namespace server
