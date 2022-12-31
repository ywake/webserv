#ifndef STRING_ARRAY_HPP
#define STRING_ARRAY_HPP

#include <string>
#include <vector>

class StringArray
{
  private:
	std::vector<std::string>  strings_;
	std::vector<const char *> c_strings_;

  public:
	StringArray(const std::vector<std::string> &strings);
	StringArray(const StringArray &rhs);
	StringArray                    &operator=(const StringArray &other);
	const std::vector<std::string> &Strings() const;
	const char *const              *CArray() const;

  private:
	void InitCstrings(const std::vector<std::string> &strings);
};

#endif
