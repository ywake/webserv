#ifndef HTTP_EXCEPTIONS_HPP
#define HTTP_EXCEPTIONS_HPP

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

		const StatusCode &GetStatusCode() const;
		const char       *what() const throw();
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

	class UriTooLongException : public HttpException
	{
	  public:
		UriTooLongException();
	};

	class NotFoundException : public HttpException
	{
	  public:
		NotFoundException();
	};

	class ForbiddenException : public HttpException
	{
	  public:
		ForbiddenException();
	};

	class MethodNotAllowedException : public HttpException
	{
	  public:
		MethodNotAllowedException();
	};

	class ContentTooLargeException : public HttpException
	{
	  public:
		ContentTooLargeException();
	};

	class InternalServerErrorException : public HttpException
	{
	  public:
		InternalServerErrorException();
	};
} // namespace http

#endif
