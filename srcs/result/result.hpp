#ifndef RESULT_HPP
#define RESULT_HPP
#include "error.hpp"

template <typename T>
class Result
{
  private:
	T     val;
	Error err;

  public:
	Result() : val(), err(){};
	Result(const T &v) : val(v), err(){};
	Result(const T &v, Error e) : val(v), err(e){};
	Result(Error e) : err(e){};
	bool IsOk()
	{
		return !err.IsErr();
	}
	bool IsErr()
	{
		return err.IsErr();
	}
	T Val()
	{
		return val;
	}
	std::string Err()
	{
		return err.Err();
	}
};

template <typename T>
class Result<T &>
{
  private:
	T     default_;
	T    &val;
	Error err;

  public:
	Result() : default_(), val(default_), err(){};
	Result(const T &v) : val(v), err(){};
	Result(const T &v, Error e) : val(v), err(e){};
	Result(Error e) : err(e){};
	bool IsOk()
	{
		return !err.IsErr();
	}
	bool IsErr()
	{
		return err.IsErr();
	}
	T Val()
	{
		return val;
	}
	std::string Err()
	{
		return err.Err();
	}
};

template <>
class Result<void>
{
  private:
	Error err;

  public:
	Result() : err(){};
	Result(Error e) : err(e){};
	bool IsOk()
	{
		return !err.IsErr();
	}
	bool IsErr()
	{
		return err.IsErr();
	}
	std::string Err()
	{
		return err.Err();
	}
};

#endif
