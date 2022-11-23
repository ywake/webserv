#ifndef HEADER_VALUE_HPP
#define HEADER_VALUE_HPP

#include <list>
#include <map>
#include <string>

#include "field_line.hpp"

class HeaderValue
{
  private:
	std::string value_;
	// std::map<std::string, std::string> param_;

  public:
	HeaderValue(const std::string &value = "");
	~HeaderValue();

	bool empty() const;
	void push_back(const FieldLine &field_line);

	HeaderValue &operator=(const HeaderValue &rhs);
	bool         operator==(const HeaderValue &rhs) const;
	bool         operator!=(const HeaderValue &rhs) const;

	const std::string &GetValue() const;
};

std::ostream &operator<<(std::ostream &os, const HeaderValue &field_value);
#endif
