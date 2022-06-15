#ifndef ERROR_HPP
#define ERROR_HPP
#include <cstring>
#include <string>

class Error
{
  private:
	bool is_err_;
	std::string err_msg_;

  public:
	Error();
	Error(int errn);
	Error(const char *msg);
	bool IsErr();
	std::string Err();
};

#endif
