#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

int child_task()
{
	return 0;
}

void parent_task(int pid)
{
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
	int pid = fork();
	switch (pid) {
	case -1:
		std::cout << "fork error" << std::endl;
		return 1;
	case 0:
		exit(child_task());
		break;
	default:
		parent_task(pid);
		break;
	}
	return 0;
}
