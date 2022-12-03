#ifndef METHOD_POOL_HPP
#define METHOD_POOL_HPP

#include <set>
#include <string>

namespace http
{
	class ImplementedMethods
	{
	  private:
		typedef std::set<std::string> Pool;

		static const Pool pool_;

	  public:
		static const std::size_t kMaxLength;
		static const std::string kGet;
		static const std::string kDelete;
		static const std::string kPost;

	  private:
		static Pool        InitPool();
		static std::size_t InitMaxLength();

	  public:
		static bool Contains(const std::string &method);
	};
} // namespace http

#endif
