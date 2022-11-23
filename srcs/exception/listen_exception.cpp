#include "listen_exception.hpp"

ListenException::ListenException(const std::string &what_arg) : std::runtime_error(what_arg) {}
