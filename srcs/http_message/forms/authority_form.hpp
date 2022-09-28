#ifndef AUTHORITY_FORM_HPP
#define AUTHORITY_FORM_HPP

#include "authority.hpp"
#include "i_target_form.hpp"
#include "thin_string.hpp"

class AuthorityForm : public ITargetForm
{
  private:
	Authority authority_;

  public:
	AuthorityForm();
	AuthorityForm(const ThinString &str);
	AuthorityForm(const Authority &authority);
	AuthorityForm(const AuthorityForm &other);
	AuthorityForm    &operator=(const AuthorityForm &other);
	bool              operator==(const AuthorityForm &other) const;
	bool              operator!=(const AuthorityForm &other) const;
	const ThinString &GetScheme() const;
	const ThinString &GetUserinfo() const;
	const ThinString &GetHost() const;
	const ThinString &GetPort() const;
	const ThinString &GetPath() const;
	const ThinString &GetQuery() const;
};

std::ostream &operator<<(std::ostream &os, const AuthorityForm &authority);

#endif
