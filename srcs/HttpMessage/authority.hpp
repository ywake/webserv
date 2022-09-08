#ifndef AUTHORITY_HPP
#define AUTHORITY_HPP

#include "ThinString.hpp"

class Authority
{
  private:
	ThinString userinfo_;
	ThinString host_;
	ThinString port_;

  public:
	Authority();
	Authority(const ThinString &authority);
	Authority &operator=(const Authority &other);
};

#endif // AUTHORITY_HPP
