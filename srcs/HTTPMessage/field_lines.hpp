#ifndef FIELD_LINES_HPP
#define FIELD_LINES_HPP
#include <map>
#include <string>

class FieldLines
{
  private:
	std::map<const std::string, std::string> field_lines_;

  public:
	FieldLines() : field_lines_() {}

	std::string &operator[](std::string field_name)
	{
		for (std::string::iterator it = field_name.begin(); it != field_name.end(); it++) {
			*it = std::tolower(*it);
		}
		return field_lines_[field_name];
	}

	bool operator==(const FieldLines &rhs) const
	{
		return field_lines_ == rhs.field_lines_;
	}

	bool operator!=(const FieldLines &rhs) const
	{
		return !(*this == rhs);
	}
};

#endif
