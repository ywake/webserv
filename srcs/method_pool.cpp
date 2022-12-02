#include "method_pool.hpp"

namespace http
{
	const std::string MethodPool::kGet    = "GET";
	const std::string MethodPool::kDelete = "DELETE";
	const std::string MethodPool::kPost   = "POST";

	const MethodPool::Pool MethodPool::pool_            = MethodPool::InitPool();
	const std::size_t      MethodPool::kMaxMethodLength = MethodPool::InitMaxLength();

	MethodPool::Pool MethodPool::InitPool()
	{
		Pool pool;

		pool.insert(kGet);
		pool.insert(kDelete);
		pool.insert(kPost);
		return pool;
	}

	std::size_t MethodPool::InitMaxLength()
	{
		std::size_t len = 0;

		for (Pool::const_iterator it = pool_.begin(); it != pool_.end(); it++) {
			if (len < it->size()) {
				len = it->size();
			}
		}
		return len;
	}

	bool MethodPool::Contains(const std::string &method)
	{
		return pool_.find(method) != pool_.end();
	}

} // namespace http
