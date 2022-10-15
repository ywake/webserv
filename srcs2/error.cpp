#include "error.hpp"

Error::Error() : is_err_(false) {}

// TODO いらんかも
Error::Error(int errn) : is_err_(true), err_msg_(strerror(errn)) {}

Error::Error(const std::string &msg) : is_err_(true), err_msg_(msg) {}

Error::Error(const char *msg) : is_err_(true), err_msg_(msg) {}

bool Error::IsErr()
{
	return is_err_;
}

std::string Error::Err()
{
	return err_msg_;
}
