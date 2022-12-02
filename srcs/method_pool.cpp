#include "method_pool.hpp"

namespace http
{
	const std::string ImplementedMethods::kGet    = "GET";
	const std::string ImplementedMethods::kDelete = "DELETE";
	const std::string ImplementedMethods::kPost   = "POST";

	const ImplementedMethods::Pool ImplementedMethods::pool_ = ImplementedMethods::InitPool();
	const std::size_t ImplementedMethods::kMaxMethodLength   = ImplementedMethods::InitMaxLength();

	ImplementedMethods::Pool ImplementedMethods::InitPool()
	{
		Pool pool;

		pool.insert(kGet);
		pool.insert(kDelete);
		pool.insert(kPost);
		return pool;
	}

	std::size_t ImplementedMethods::InitMaxLength()
	{
		std::size_t len = 0;

		for (Pool::const_iterator it = pool_.begin(); it != pool_.end(); it++) {
			if (len < it->size()) {
				len = it->size();
			}
		}
		return len;
	}

	bool ImplementedMethods::Contains(const std::string &method)
	{
		return pool_.find(method) != pool_.end();
	}

} // namespace http
