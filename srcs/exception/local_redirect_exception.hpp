#ifndef LOCAL_REDIRECT_EXCEPTION
#define LOCAL_REDIRECT_EXCEPTION

#include <stdexcept>
#include <string>

namespace cgi
{
	class LocalRedirectException : public std::exception
	{
	  public:
		LocalRedirectException(const std::string &path, const std::string query);
		const std::string &Path() const;
		const std::string &Query() const;
		~LocalRedirectException() throw();

	  private:
		std::string path_;
		std::string query_;
	};
} // namespace cgi

#endif
