#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>

#define ON true

template <typename T, typename U>
void log(T title, U contents)
{
	if (ON) {
		std::cout << "[" << title << "] " << contents << std::endl;
	}
}

#endif
