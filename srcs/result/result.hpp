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
	Result(const Error &e) : err_(e){};
	bool IsOk()
	{
		return !err_.IsErr();
	}
	bool IsErr()
	{
		return err_.IsErr();
	}
	T Val()
	{
		return val_;
	}
	std::string Err()
	{
		return err_.Err();
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
	Result(T &v) : val_(v), err_(){};
	Result(const T &v) : val_(v), err_(){};
	Result(T &v, Error &e) : val_(v), err_(e){};
	Result(const T &v, const Error &e) : val_(v), err_(e){};
	Result(Error &e) : default_(), val_(default_), err_(e){};
	Result(const Error &e) : default_(), val_(default_), err_(e){};
	bool IsOk()
	{
		return !err_.IsErr();
	}
	bool IsErr()
	{
		return err_.IsErr();
	}
	T Val()
	{
		return val_;
	}
	std::string Err()
	{
		return err_.Err();
	}
};

template <>
class Result<void>
{
  private:
	Error err_;

  public:
	Result() : err_(){};
	Result(const Error &e) : err_(e){};
	bool IsOk()
	{
		return !err_.IsErr();
	}
	bool IsErr()
	{
		return err_.IsErr();
	}
	std::string Err()
	{
		return err_.Err();
	}
};

#endif
