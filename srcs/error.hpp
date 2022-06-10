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
	Error() : is_err_(false) {}
	Error(int errn) : is_err_(true), err_msg_(strerror(errn)) {}
	Error(const char *msg) : is_err_(true), err_msg_(msg) {}
	bool IsErr()
	{
		return is_err_;
	}
	std::string Err()
	{
		return err_msg_;
	}
};

#endif
