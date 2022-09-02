#ifndef WEBSERV_UTILS_HPP
#define WEBSERV_UTILS_HPP

#include "ThinString.hpp"
#include "result.hpp"
#include <string>

std::vector<ThinString> Split(const ThinString &str, const std::string delim);
Result<long> StrToLong(const std::string &str);

#endif
