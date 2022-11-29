#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

int child_task(int fd[2])
{
	(void)fd;

	return 0;
}

void parent_task(int pid, int fd[2])
{
	(void)fd;
	int status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		std::cout << "Child exited with status " << WEXITSTATUS(status) << std::endl;
	else if (WIFSIGNALED(status))
		std::cout << "Child exited with signal " << WTERMSIG(status) << std::endl;
	else
		std::cout << "Child exited with unknown status" << std::endl;
}

int main(void)
{
	int fd[2];
	int ret = socketpair(AF_UNIX, SOCK_STREAM, 0, fd);
	if (ret == -1) {
		std::cout << "socketpair error" << std::endl;
		return 1;
	}
	int pid = fork();
	switch (pid) {
	case -1:
		std::cout << "fork error" << std::endl;
		return 1;
	case 0:
		exit(child_task(fd));
		break;
	default:
		parent_task(pid, fd);
		break;
	}
	close(fd[0]);
	close(fd[1]);
	return 0;
}
