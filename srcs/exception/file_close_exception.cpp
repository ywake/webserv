#include "file_close_exception.hpp"

FileCloseException::FileCloseException(const std::string &what_arg) : std::runtime_error(what_arg)
{}
