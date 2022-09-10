#ifndef ABSOLUTE_FORM_HPP
#define ABSOLUTE_FORM_HPP

#include "absolute_uri.hpp"
#include "thin_string.hpp"

class AbsoluteForm : public ITargetForm
{
  private:
	AbsoluteUri uri_;

  public:
	AbsoluteForm(const ThinString request_target);

	const ThinString &GetScheme() const;
	const ThinString &GetUserinfo() const;
	const ThinString &GetHost() const;
	const ThinString &GetPort() const;
	const ThinString &GetPath() const;
	const ThinString &GetQuery() const;
};

#endif
