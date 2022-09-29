#ifndef HTTP_EXCEPTION_HPP
#define HTTP_EXCEPTION_HPP

#include "status_code.hpp"
#include <stdexcept>

namespace http
{
	class HttpException : public std::exception
	{
	  private:
		StatusCode status_code_;

	  public:
		HttpException(StatusCode status_code);

		StatusCode  GetStatusCode() const;
		const char *what() const throw();
	};

	class BadRequestException : public HttpException
	{
	  public:
		BadRequestException();
	};

	class NotImplementedException : public HttpException
	{
	  public:
		NotImplementedException();
	};
} // namespace http

#endif
