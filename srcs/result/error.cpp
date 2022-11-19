#include "error.hpp"

Error::Error() : is_err_(false) {}

Error::Error(int errn) : is_err_(true), err_msg_(strerror(errn)) {}

Error::Error(const char *msg) : is_err_(true), err_msg_(msg) {}

Error::Error(const std::string &msg) : is_err_(true), err_msg_(msg) {}

Error::Error(const Error &err) : is_err_(err.is_err_), err_msg_(err.err_msg_) {}

bool Error::IsErr() const
{
	return is_err_;
}

const std::string &Error::Err() const
{
	return err_msg_;
}

Error &Error::operator=(const Error &rhs)
{
	if (this == &rhs) {
		return *this;
	}
	this->is_err_  = rhs.is_err_;
	this->err_msg_ = rhs.err_msg_;
	return *this;
}

bool Error::operator==(const Error &rhs) const
{
	return is_err_ == rhs.is_err_ && err_msg_ == rhs.err_msg_;
}

bool Error::operator!=(const Error &rhs) const
{
	return !(*this == rhs);
}

std::ostream &operator<<(std::ostream &os, const Error &error)
{
	std::string title = error.IsErr() ? "Error!" : "Success";
	os << title << "(" + error.Err() + ")";
	return os;
}
