#ifndef DEBUG_HPP
#define DEBUG_HPP

#include "epoll.hpp"
#include "result.hpp"
#include <iomanip>
#include <iostream>

#define COL_END "\x1b[0m"
#define COL_BOLD "\x1b[1m"
#define COL_THIN "\x1b[2m"
#define COL_ITALIC "\x1b[3m"
#define COL_U_LINE "\x1b[4m"
#define COL_BLACK "\x1b[30m"
#define COL_RED "\x1b[31m"
#define COL_GREEN "\x1b[32m"
#define COL_YELLOW "\x1b[33m"
#define COL_BLUE "\x1b[34m"
#define COL_PURPLE "\x1b[35m"
#define COL_CYAN "\x1b[36m"
#define COL_WHITE "\x1b[37m"

#ifdef NDEBUG
inline void log(...) {}
inline void loge(...) {}
#define DBG_INFO ((void)0)
#else
std::string BaseName(std::string s);
#define DBG_INFO                                                                                   \
	(std::cerr << COL_BOLD << BaseName(__FILE__) << COL_END << "::" << __func__ << " " << __LINE__ \
			   << std::endl)
template <typename T, typename U>
void log(const T &title, const U &contents, const std::string &delim = "")
{
	std::cerr << "[ " << std::setw(13) << std::left << title << "] " << delim << contents
			  << std::endl;
}

void log(const std::string &title);
void loge(const Error &err);
void log(const io_multiplexer::ErrEvents &err);
#endif
#endif
