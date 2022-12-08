#include <iostream>

#include "debug.hpp"
#include "epoll.hpp"
#include "instruction.hpp"

void loge(const Error &err)
{
	std::cerr << err << std::endl;
}

void log(const io_multiplexer::ErrEvents &err)
{
	for (io_multiplexer::ErrEvents::const_iterator it = err.begin(); it != err.end(); ++it) {
		std::cerr << it->Val() << std::endl;
		std::cerr << it->Err() << std::endl << std::endl;
	}
}

void log(const std::string &title)
{
	std::cerr << std::setw(13) << std::left << title << std::endl;
}

std::string BaseName(std::string s)
{
	std::string name;
	std::size_t pos;

	pos = s.rfind('/');
	if (pos == std::string::npos) {
		name = s;
	} else {
		name = s.substr(pos + 1, s.size() - pos - 1);
	}

	pos = name.rfind('.');
	if (pos == std::string::npos) {
		return name;
	}
	return name.substr(0, pos);
}
