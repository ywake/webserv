#ifndef ERROR_HPP
#define ERROR_HPP
#include <cstring>
#include <string>

class Error
{
  private:
	bool		is_err_;
	std::string err_msg_;

  public:
	Error();
	Error(int errn);
	Error(const char *msg);
	bool		IsErr() const;
	std::string Err() const;

	Error &operator=(const Error &rhs);
	bool   operator==(const Error &rhs) const;
	bool   operator!=(const Error &rhs) const;
};

std::ostream &operator<<(std::ostream &os, const Error &error);

#endif
