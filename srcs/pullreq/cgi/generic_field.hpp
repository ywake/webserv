#ifndef GENERIC_FIELD_HPP
#define GENERIC_FIELD_HPP

#include "thin_string.hpp"

namespace cgi
{
	class GenericField
	{
	  private:
		ThinString field_name_;
		ThinString field_value_;

	  public:
		GenericField(const ThinString &line);
		GenericField(const ThinString &line);
		const ThinString &GetFieldName() const;
		const ThinString &GetFieldValue() const;

		bool operator==(const GenericField &rhs) const;
		bool operator!=(const GenericField &rhs) const;
	};

	std::ostream &operator<<(std::ostream &os, const GenericField &generic_field);
} // namespace cgi

#endif // GENERIC_FIELD_HPP
