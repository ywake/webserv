#include "config_exceptions.hpp"

namespace conf
{
	ConfigException::ConfigException(const std::string &what_arg) : std::runtime_error(what_arg) {}
} // namespace conf
