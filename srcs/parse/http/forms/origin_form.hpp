#ifndef ORIGIN_FORM_HPP
#define ORIGIN_FORM_HPP

#include "thin_string.hpp"

class OriginForm
{
  private:
	ThinString path_;
	ThinString query_;

  public:
	OriginForm();
	OriginForm(const ThinString &str);
	OriginForm(const ThinString &path, const ThinString &query);
	OriginForm(const OriginForm &other);

	OriginForm &operator=(const OriginForm &other);
	bool        operator==(const OriginForm &other) const;
	bool        operator!=(const OriginForm &other) const;

	const ThinString &GetPath() const;
	const ThinString &GetQuery() const;
};

std::ostream &operator<<(std::ostream &os, const OriginForm &origin);

#endif
