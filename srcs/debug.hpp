#ifndef LOG_HPP
#define LOG_HPP

#include <iomanip>
#include <iostream>

#define DEBUG true

template <typename T, typename U>
void log(T title, U contents)
{
	if (DEBUG) {
		std::cout << "[ " << std::setw(13) << std::left << title << "] " << contents << std::endl;
	}
}

#endif
