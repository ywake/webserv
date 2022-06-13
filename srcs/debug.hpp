#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>

#define DEBUG true

template <typename T, typename U>
void log(T title, U contents)
{
	if (DEBUG) {
		std::cout << "[" << title << "] " << contents << std::endl;
	}
}

#endif
