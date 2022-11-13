#ifndef FILE_CLOSE_EXCEPTION_HPP
#define FILE_CLOSE_EXCEPTION_HPP

#include <stdexcept>

class FileCloseException : public std::runtime_error
{
  public:
	FileCloseException(const std::string &what_arg);
};

#endif
