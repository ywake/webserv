#ifndef ABSOLUTE_FORM_HPP
#define ABSOLUTE_FORM_HPP

#include "absolute_uri.hpp"
#include "thin_string.hpp"

class AbsoluteForm
{
  private:
	AbsoluteUri uri_;

  public:
	AbsoluteForm(const ThinString request_target);
	AbsoluteForm(const AbsoluteUri uri);

	const ThinString &GetScheme() const;
	const ThinString &GetUserinfo() const;
	const ThinString &GetHost() const;
	const ThinString &GetPort() const;
	const ThinString &GetPath() const;
	const ThinString &GetQuery() const;

	bool operator==(const AbsoluteForm &rhs) const;
	bool operator!=(const AbsoluteForm &rhs) const;

	const AbsoluteUri &GetUri() const;

  private:
	bool IsValidHttpScheme();
};

std::ostream &operator<<(std::ostream &os, const AbsoluteForm &form);

#endif
