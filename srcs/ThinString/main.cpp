#include "ThinString.hpp"
#include <iostream>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

// typedef ThinString String;
typedef std::string String;

String deep(String s)
{
	return s;
}

#include <sys/time.h>
#include <time.h>
#define SIZE 10000000
#define SEC_TO_NSEC(sec) (sec * 1000000000)
#define NSEC_TO_USEC(sec) (sec / 1000)
#define SEC_TO_USEC(sec) (sec * 1000 * 1000)

static long get_nsec(const struct timespec ts)
{
	return SEC_TO_NSEC(ts.tv_sec) + ts.tv_nsec;
}

long get_current_nsec()
{
	struct timespec now;

	clock_gettime(CLOCK_MONOTONIC, &now);
	return get_nsec(now);
}

long get_current_usec()
{
	return NSEC_TO_USEC(get_current_nsec());
}

int main(int argc, char **argv)
{
	// std::vector<ThinString> a;
	// for (int i = 0; i < 10000 * 100; i++) {
	// 	std::string s;
	// 	std::cin >> s;
	// 	a.push_back(s);
	// }
	int size = atoi(argv[1]);
	char *p = (char *)malloc(size);
	p[size - 1] = 0;
	memset(p, 'a', size - 1);
	String s;

	long start = get_current_usec();
	for (size_t i = 0; i < 1000; i++)
		s = deep(p);
	long end = get_current_usec();
	long elapsed = end - start;
	std::cout << elapsed << std::endl;
}
