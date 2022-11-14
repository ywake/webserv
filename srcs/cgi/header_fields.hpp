#ifndef HEADER_FIELDS_HPP
#define HEADER_FIELDS_HPP

#include "generic_field.hpp"
#include "header_value.hpp"
#include <list>
#include <vector>

namespace cgi
{
	class HeaderFields
	{
	  public:
		typedef std::list<HeaderValue>               Values;
		typedef std::map<const std::string, Values > Headers;

	  private:
		typedef std::list<GenericField> Lines;

	  private:
		Headers header_fields_;

	  public:
		HeaderFields();
		HeaderFields(const ThinString &str);
		HeaderFields(const Headers &header_fields);

		bool          operator==(const HeaderFields &rhs) const;
		bool          operator!=(const HeaderFields &rhs) const;
		Values       &operator[](const std::string &field_name);
		const Values &operator[](const std::string &field_name) const;

		std::size_t size() const;
		/**
		 * @exception std::out_of_range
		 */
		Values &at(const std::string &field_name);
		/**
		 * @exception std::out_of_range
		 */
		const Values &at(const std::string &field_name) const;

		void           Add(const std::string &line);
		const Headers &GetMap() const;
		bool           Contains(const std::string &field_name) const;
	};

	std::ostream &operator<<(std::ostream &out, const HeaderFields &header_fields);

} // namespace cgi

#endif // HEADER_FIELDS_HPP
