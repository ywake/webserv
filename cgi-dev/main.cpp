#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

int  err_check(int ret);
void exec_cgi();
void child_task(int fd[2]);
void parent_task(int pid, int fd[2]);

int main(void)
{
	// TODO: リソースの存在確認
	exec_cgi();

	return 0;
}

int err_check(int ret)
{
	if (ret == -1) {
		std::perror("Error");
		std::exit(1);
	}
	return ret;
}

void exec_cgi()
{
	int fd[2];
	err_check(socketpair(AF_UNIX, SOCK_STREAM, 0, fd));
	int pid = err_check(fork());
	switch (pid) {
	case 0:
		child_task(fd);
		break;
	default:
		parent_task(pid, fd);
		break;
	}
	err_check(close(fd[0]));
	err_check(close(fd[1]));
}

void child_task(int fd[2])
{
	err_check(close(fd[0]));
	err_check(dup2(fd[1], STDOUT_FILENO));
	err_check(close(fd[1]));

	char *const argv[] = {(char *)"/usr/bin/python3", (char *)"index.py", NULL};
	err_check(execve("/usr/bin/python3", argv, NULL));
}

void parent_task(int pid, int fd[2])
{
	int status;

	waitpid(pid, &status, WNOHANG);
	if (WIFEXITED(status))
		std::cout << "Child exited with status " << WEXITSTATUS(status) << std::endl;
	else
		std::cout << "Child exited with unknown status" << std::endl;

	err_check(close(fd[1]));

	char buf[1024];
	int  res = err_check(read(fd[0], buf, 1023));
	buf[res] = '\0';
	std::cout << buf << std::endl;
}
