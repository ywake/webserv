#ifndef FIELD_SECTION_HPP
#define FIELD_SECTION_HPP

#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "field_line.hpp"
#include "header_value.hpp"

namespace http
{
	class FieldSection
	{
		// [RFC 9110 5.3] 同一名ヘッダは結合可能
	  public:
		typedef std::list<HeaderValue>              Values;
		typedef std::map<const std::string, Values> FieldLines;

	  private:
		typedef std::vector<ThinString> StringAry;

	  private:
		FieldLines field_lines_;

	  public:
		FieldSection();
		FieldSection(const FieldSection &other);
		FieldSection(const FieldLines &field_lines);

		FieldSection &operator=(const FieldSection &rhs);
		bool          operator==(const FieldSection &rhs) const;
		bool          operator!=(const FieldSection &rhs) const;
		Values       &operator[](const std::string &field_name);
		const Values &operator[](const std::string &field_name) const;

		/**
		 * @exception std::out_of_range
		 */
		Values &at(const std::string &field_name);
		/**
		 * @exception std::out_of_range
		 */
		const Values &at(const std::string &field_name) const;

		const FieldLines &GetMap() const;
		void              Append(const FieldLine &line);

		bool Contains(const std::string &field_name) const;
	};
	std::ostream &operator<<(std::ostream &os, const FieldSection &field_section);
} // namespace http

#endif
