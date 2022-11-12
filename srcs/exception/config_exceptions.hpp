#include <stdexcept>

namespace conf
{
	class ConfigException : public std::runtime_error
	{
	  public:
		ConfigException(const std::string &what_arg);
	};
} // namespace conf
