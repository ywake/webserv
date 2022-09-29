#ifndef AUTHORITY_FORM_HPP
#define AUTHORITY_FORM_HPP

#include "host_port.hpp"
#include "i_target_form.hpp"
#include "thin_string.hpp"

class AuthorityForm : public ITargetForm
{
  private:
	http_abnf::HostPort host_port_;

  public:
	AuthorityForm();
	AuthorityForm(const ThinString &str);
	AuthorityForm(const http_abnf::HostPort &host_port);
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
