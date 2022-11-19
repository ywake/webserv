#include <iostream>

#include "debug.hpp"

void log(Error &err)
{
	if (DEBUG) {
		std::cerr << err << std::endl;
	}
}
