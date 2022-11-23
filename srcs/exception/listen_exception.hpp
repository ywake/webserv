#ifndef FILE_CLOSE_EXCEPTION_20COPY_HPP
#define FILE_CLOSE_EXCEPTION_20COPY_HPP

#include <stdexcept>

class ListenException : public std::runtime_error
{
  public:
	ListenException(const std::string &what_arg);
};

#endif
