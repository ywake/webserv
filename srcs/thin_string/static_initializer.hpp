#ifndef STATIC_INITIALIZER_HPP
#define STATIC_INITIALIZER_HPP
#include "thin_string.hpp"

template <typename T>
class StaticInitializer
{
  public:
	StaticInitializer(T **ptr)
	{
		*ptr = new T();
	};
};

#endif
