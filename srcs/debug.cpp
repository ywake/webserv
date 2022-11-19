#include <iostream>

#include "debug.hpp"

void log(const Error &err)
{
	if (DEBUG) {
		std::cerr << err << std::endl;
	}
}
