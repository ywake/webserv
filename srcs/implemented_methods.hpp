#ifndef METHOD_POOL_HPP
#define METHOD_POOL_HPP

#include <set>
#include <string>
#include <vector>
namespace http
{
	class ImplementedMethods
	{
	  private:
		typedef std::set<std::string>    Pool;
		typedef std::vector<std::string> Array;

		static const Pool  kPool;
		static const Array kArray;

	  public:
		static const std::size_t kMaxLength;

	  private:
		static std::size_t InitMaxLength();

	  public:
		static bool         Contains(const std::string &method);
		static const Array &GetMethods();
	};
} // namespace http

#endif
