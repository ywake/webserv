#include "local_redirect_exception.hpp"

namespace cgi
{
	LocalRedirectException::LocalRedirectException(const std::string &path, const std::string query)
		: std::exception(), path_(path), query_(query)
	{}

	LocalRedirectException::~LocalRedirectException() throw() {}

	const std::string &LocalRedirectException::Path() const
	{
		return path_;
	}

	const std::string &LocalRedirectException::Query() const
	{
		return query_;
	}

} // namespace cgi
