#ifndef FIELD_VALUE_HPP
#define FIELD_VALUE_HPP

#include <list>
#include <map>
#include <string>

#include "field_line.hpp"

class FieldValue
{
  private:
	class FieldValueElem
	{
	  private:
		std::string value_;
		// std::map<std::string, std::string> param_;

	  public:
		FieldValueElem(const std::string &value);
		~FieldValueElem();
	};

  private:
	std::list<FieldValueElem> values_;

  public:
	FieldValue();
	~FieldValue();

	bool empty() const;
	void push_back(const FieldLine &field_line);

	bool operator==(const FieldValue &rhs) const;
	bool operator!=(const FieldValue &rhs) const;
};

std::ostream &operator<<(std::ostream &os, const FieldValue &field_value);
#endif
