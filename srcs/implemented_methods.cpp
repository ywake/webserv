#include "implemented_methods.hpp"
#include "http_define.hpp"
#include "webserv_utils.hpp"

namespace http
{
	namespace methods
	{
		const std::string kGet    = "GET";
		const std::string kDelete = "DELETE";
		const std::string kPost   = "POST";
	} // namespace methods

	// C++98ではコンテナのリスト初期化が使えないので
	static const char *kImplementedMethods[] = {
		methods::kGet.c_str(),
		methods::kDelete.c_str(),
		methods::kPost.c_str(),
	};

	static const char **kImplementedMethodsEnd =
		kImplementedMethods + ARRAY_SIZE(kImplementedMethods);

	const ImplementedMethods::Pool ImplementedMethods::kPool =
		ImplementedMethods::Pool(kImplementedMethods, kImplementedMethodsEnd);

	const ImplementedMethods::Array ImplementedMethods::kArray =
		ImplementedMethods::Array(kImplementedMethods, kImplementedMethodsEnd);

	const std::size_t ImplementedMethods::kMaxLength = ImplementedMethods::InitMaxLength();

	std::size_t ImplementedMethods::InitMaxLength()
	{
		std::size_t len = 0;

		for (Pool::const_iterator it = kPool.begin(); it != kPool.end(); it++) {
			if (len < it->size()) {
				len = it->size();
			}
		}
		return len;
	}

	bool ImplementedMethods::Contains(const std::string &method)
	{
		return kPool.find(method) != kPool.end();
	}

	const ImplementedMethods::Array &ImplementedMethods::GetMethods()
	{
		return kArray;
	}

} // namespace http
