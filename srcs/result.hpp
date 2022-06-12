#ifndef RESULT_HPP
#define RESULT_HPP
#include "error.hpp"

template <typename T>
class Result
{
  private:
	T val;

  public:
	Error err;
	Result()
		: val(), err(){};
	Result(T v)
		: val(v), err(){};
	Result(T v, Error e)
		: val(v), err(e){};
	Result(Error e)
		: err(e){};
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
  public:
	Error err;
	Result()
		: err(){};
	Result(Error e)
		: err(e){};
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
