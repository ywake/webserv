#ifndef ORIGIN_FORM_HPP
#define ORIGIN_FORM_HPP

#include "i_target_form.hpp"
#include "thin_string.hpp"

class OriginForm : public ITargetForm
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
	bool operator==(const OriginForm &other) const;
	bool operator!=(const OriginForm &other) const;

	const ThinString &GetScheme() const;
	const ThinString &GetUserinfo() const;
	const ThinString &GetHost() const;
	const ThinString &GetPort() const;
	const ThinString &GetPath() const;
	const ThinString &GetQuery() const;
};

std::ostream &operator<<(std::ostream &os, const OriginForm &origin);

#endif
