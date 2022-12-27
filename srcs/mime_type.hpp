#ifndef MIME_TYPE_HPP
#define MIME_TYPE_HPP

#include <map>
#include <string>

namespace http
{
	class MimeType
	{
	  private:
		typedef std::map<std::string, std::string> Types;
		static const Types                         mime_types_;

	  public:
		static std::string GetMimeType(const std::string &path);

	  private:
		static std::map<std::string, std::string> InitMimeTypes();
	};
} // namespace http

#endif
