#include "webserv_utils.hpp"
#include <unistd.h>

void Xpipe(int *fds)
{
	if (pipe(fds) < 0) {
		exit(EXIT_FAILURE);
	}
}

void Xclose(int fd)
{
	if (close(fd) == -1) {
		exit(EXIT_FAILURE);
	}
}

void Xdup2(int oldfd, int newfd)
{
	if (dup2(oldfd, newfd) == -1) {
		exit(EXIT_FAILURE);
	}
}
