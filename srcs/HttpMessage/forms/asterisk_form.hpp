#ifndef ASTERISK_FORM_HPP
#define ASTERISK_FORM_HPP

#include "i_target_form.hpp"
#include "thin_string.hpp"

class AsteriskForm : public ITargetForm
{
  public:
	AsteriskForm(const ThinString &str);

	const ThinString &GetScheme() const;
	const ThinString &GetUserinfo() const;
	const ThinString &GetHost() const;
	const ThinString &GetPort() const;
	const ThinString &GetPath() const;
	const ThinString &GetQuery() const;
};

#endif
