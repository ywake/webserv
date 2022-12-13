#include "field_section.hpp"
#include "webserv_utils.hpp"

namespace http
{
	FieldSection::FieldSection() : field_lines_() {}

	FieldSection::FieldSection(const FieldSection &other)
	{
		*this = other;
	}

	FieldSection::FieldSection(const FieldLines &field_lines) : field_lines_(field_lines) {}

	FieldSection &FieldSection::operator=(const FieldSection &rhs)
	{
		if (&rhs == this) {
			return *this;
		}
		field_lines_ = rhs.field_lines_;
		return *this;
	}

	void FieldSection::Append(const FieldLine &line)
	{
		const std::string &name  = utils::ToLowerString(line.GetFieldName().ToString());
		const std::string &value = line.GetFieldValue().ToString();
		field_lines_[name].push_back(HeaderValue(value));
	}

	bool FieldSection::operator==(const FieldSection &rhs) const
	{
		return &rhs == this || field_lines_ == rhs.field_lines_;
	}

	bool FieldSection::operator!=(const FieldSection &rhs) const
	{
		return !(*this == rhs);
	}

	FieldSection::Values &FieldSection::operator[](const std::string &field_name)
	{
		return field_lines_[utils::ToLowerString(field_name)];
	}

	const FieldSection::Values &FieldSection::operator[](const std::string &field_name) const
	{
		static const Values empty;

		try {
			return field_lines_.at(utils::ToLowerString(field_name));
		} catch (const std::out_of_range &e) {
			return empty;
		}
	}

	FieldSection::Values &FieldSection::at(const std::string &field_name)
	{
		return field_lines_.at(utils::ToLowerString(field_name));
	}

	const FieldSection::Values &FieldSection::at(const std::string &field_name) const
	{
		return field_lines_.at(utils::ToLowerString(field_name));
	}

	const FieldSection::FieldLines &FieldSection::GetMap() const
	{
		return field_lines_;
	}

	bool FieldSection::Contains(const std::string &field_name) const
	{
		FieldLines::const_iterator it = field_lines_.find(utils::ToLowerString(field_name));
		return it != field_lines_.end() && !it->second.empty();
	}

	std::ostream &operator<<(std::ostream &os, const FieldSection &field_section)
	{
		FieldSection::FieldLines field_lines = field_section.GetMap();
		for (FieldSection::FieldLines::const_iterator it = field_lines.begin();
			 it != field_lines.end();
			 it++) {
			const std::string &name   = it->first;
			const HeaderValue &values = it->second.front(); // TODO fix empty, multi val
			os << name << ": " << values << "\n";
		}
		return os;
	}
} // namespace http
