#include "thin_string.hpp"

static const std::string kWhiteSpaces = " \t";

// TODO test
namespace http_abnf
{
	static bool IsWhiteSpaceOnly(const ThinString &str);

	// *( SP / HTAB )
	bool IsOws(const ThinString &str)
	{
		return str.empty() || IsWhiteSpaceOnly(str);
	}

	//  1*( SP / HTAB )
	bool IsRws(const ThinString &str)
	{
		return !str.empty() && IsWhiteSpaceOnly(str);
	}

	static bool IsWhiteSpaceOnly(const ThinString &str)
	{
		for (std::size_t i = 0; i < str.size(); i++) {
			if (kWhiteSpaces.find(str.at(i)) == ThinString::npos) {
				return false;
			};
		}
		return true;
	}

} // namespace http_abnf
