#ifndef RESULT_HPP
#define RESULT_HPP

#include "error.hpp"

template <typename T>
class Result
{
  private:
	T     val_;
	Error err_;

  public:
	Result() : val_(), err_(){};
	Result(const T &v) : val_(v), err_(){};
	Result(const T &v, const Error &e) : val_(v), err_(e){};
	Result(const Error &e) : val_(), err_(e.Err()){};
	Result(const Result &r) : val_(r.val_), err_(r.err_){};
	bool IsOk() const
	{
		return !err_.IsErr();
	}
	bool IsErr() const
	{
		return err_.IsErr();
	}
	const T &Val() const
	{
		return val_;
	}
	T &Val()
	{
		return val_;
	}
	const std::string &ErrMsg() const
	{
		return err_.Err();
	}
	const Error &Err() const
	{
		return err_;
	}
	Result &operator=(const Result &r)
	{
		if (this != &r) {
			val_ = r.val_;
			err_ = r.err_;
		}
		return *this;
	}
};

template <typename T>
class Result<T &>
{
  private:
	T     default_;
	T    &val_;
	Error err_;

  public:
	Result() : default_(), val_(default_), err_(){};
	Result(const T &v) : val_(v), err_(){};
	Result(const T &v, const Error &e) : val_(v), err_(e){};
	Result(const Error &e) : default_(), val_(default_), err_(e.Err()){};
	Result(const Result &r) : val_(r.val_), err_(r.err_){};
	bool IsOk() const
	{
		return !err_.IsErr();
	}
	bool IsErr() const
	{
		return err_.IsErr();
	}
	T &Val()
	{
		return val_;
	}
	const T &Val() const
	{
		return val_;
	}
	const std::string &ErrMsg() const
	{
		return err_.Err();
	}
	const Error &Err() const
	{
		return err_;
	}
	Result &operator=(const Result &r)
	{
		if (this != &r) {
			val_ = r.val_;
			err_ = r.err_;
		}
		return *this;
	}
};

template <>
class Result<void>
{
  private:
	Error err_;

  public:
	Result() : err_(){};
	Result(const Error &e) : err_(e.Err()){};
	Result(const Result &r) : err_(r.err_){};
	bool IsOk() const
	{
		return !err_.IsErr();
	}
	bool IsErr() const
	{
		return err_.IsErr();
	}
	const std::string &ErrMsg() const
	{
		return err_.Err();
	}
	const Error &Err() const
	{
		return err_;
	}
	Result &operator=(const Result &r)
	{
		if (this != &r) {
			err_ = r.err_;
		}
		return *this;
	}
};

#endif
