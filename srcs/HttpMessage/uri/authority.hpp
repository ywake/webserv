#ifndef AUTHORITY_HPP
#define AUTHORITY_HPP

#include "thin_string.hpp"

class Authority
{
  private:
	ThinString userinfo_;
	ThinString host_;
	ThinString port_;

  public:
	Authority();
	Authority(const ThinString &authority);
	Authority(const ThinString &userinfo, const ThinString &host, const ThinString &port);
	Authority(const Authority &other);

	Authority &operator=(const Authority &other);
	bool operator==(const Authority &other) const;
	bool operator!=(const Authority &other) const;

	const ThinString &GetUserinfo() const;
	const ThinString &GetHost() const;
	const ThinString &GetPort() const;
};

std::ostream &operator<<(std::ostream &os, const Authority &authority);

#endif
