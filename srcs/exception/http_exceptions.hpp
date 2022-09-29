#ifndef HTTP_EXCEPTION_HPP
#define HTTP_EXCEPTION_HPP

#include <stdexcept>

class ParseErrorException : public std::runtime_error
{
  public:
	ParseErrorException();
};

#endif
