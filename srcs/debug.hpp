#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iomanip>
#include <iostream>

#include "result.hpp"

#define DEBUG true

template <typename T, typename U>
void log(T title, U contents)
{
	if (DEBUG) {
		std::cout << "[ " << std::setw(13) << std::left << title << "] " << contents << std::endl;
	}
}

void log(Error &err);
#endif
