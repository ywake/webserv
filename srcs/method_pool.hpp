#ifndef METHOD_POOL_HPP
#define METHOD_POOL_HPP

#include <set>
#include <string>

namespace http
{
	class MethodPool
	{
	  private:
		typedef std::set<std::string> Pool;

		static const Pool pool_;

	  public:
		static const std::size_t kMaxMethodLength;

	  private:
		static Pool        InitPool();
		static std::size_t InitMaxLength();

	  public:
		static bool Contains(const std::string &method);
	};
} // namespace http

#endif
