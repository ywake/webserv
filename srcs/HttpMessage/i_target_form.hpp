#ifndef I_TARGET_FORM_HPP
#define I_TARGET_FORM_HPP

#include "thin_string.hpp"

class ITargetForm
{
  public:
	virtual const ThinString &GetScheme() const = 0;
	virtual const ThinString &GetUserinfo() const = 0;
	virtual const ThinString &GetHost() const = 0;
	virtual const ThinString &GetPort() const = 0;
	virtual const ThinString &GetPath() const = 0;
	virtual const ThinString &GetQuery() const = 0;
};

#endif
