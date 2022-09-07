#ifndef AUTHORITY_HPP
#define AUTHORITY_HPP

#include "ThinString.hpp"

class Authority
{
  private:
	typedef struct Tokens {
		ThinString userinfo;
		ThinString host;
		ThinString port;
	} Tokens;

  private:
	std::string userinfo_;
	std::string host_;
	std::string port_;

  public:
	Authority(ThinString authority);

  private:
	Tokens TokenizeAuthority(ThinString authority);
};

#endif // AUTHORITY_HPP
