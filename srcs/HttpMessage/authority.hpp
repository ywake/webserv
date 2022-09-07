#ifndef AUTHORITY_HPP
#define AUTHORITY_HPP

#include "ThinString.hpp"

class Authority
{
  private:
	std::string userinfo_;
	std::string host_;
	std::string port_;

  public:
	Authority(ThinString authority);
};

#endif // AUTHORITY_HPP
