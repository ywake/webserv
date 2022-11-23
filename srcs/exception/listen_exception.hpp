#ifndef FILE_CLOSE_EXCEPTION_HPP
#define FILE_CLOSE_EXCEPTION_HPP

#include <stdexcept>

class ListenException : public std::runtime_error
{
  public:
	ListenException(const std::string &what_arg);
};

#endif
