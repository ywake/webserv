#include <iostream>

#include "debug.hpp"
#include "epoll.hpp"
#include "instruction.hpp"

void log(const Error &err)
{
	if (DEBUG) {
		std::cerr << err << std::endl;
	}
}

void log(const io_multiplexer::ErrEvents &err)
{
	if (DEBUG == false) {
		return;
	}
	for (io_multiplexer::ErrEvents::const_iterator it = err.begin(); it != err.end(); ++it) {
		std::cerr << it->Val() << std::endl;
		std::cerr << it->Err() << std::endl << std::endl;
	}
}
