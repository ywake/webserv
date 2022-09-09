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

	const ThinString &GetUserinfo() const;
	const ThinString &GetHost() const;
	const ThinString &GetPort() const;
};

#endif // AUTHORITY_HPP
