#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

#include "i_request.hpp"
#include "location_conf.hpp"

int  ErrCheck(int ret);
bool IsAccessible(const std::string &path);
void ExecCgi();
void ChildTask(int fd[2]);
void ParentTask(int pid, int fd[2]);

int main(int argc, char *argv[], char *envp[])
{
	server::IRequest   resource;
	conf::LocationConf conf;

	std::string resource_path = conf.GetRoot() + request.Path();
	if (IsAccessible(resource_path) == false) {
		return EXIT_FAILURE; // 404
	}
	SetMetaEnv(resource, envp);
	char **cmd_line_arg = CreateCmdLineArg(resource);
	InputBody(resource);
	ExecCgi(cmd_line_arg, envp);
	ReadCgiOutput();
	ParseCgiOutput();
	return 0;
}

int ErrCheck(int ret)
{
	if (ret == -1) {
		std::perror("Error");
		std::exit(1);
	}
	return ret;
}

bool IsAccessible(const std::string &path)
{
	if (access(path.c_str(), R_OK) != 0) {
		return false;
	}
	return true;
}

void ExecCgi()
{
	int fd[2];
	ErrCheck(socketpair(AF_UNIX, SOCK_STREAM, 0, fd));
	int pid = ErrCheck(fork());
	switch (pid) {
	case 0:
		ChildTask(fd);
		break;
	default:
		ParentTask(pid, fd);
		break;
	}
	ErrCheck(close(fd[0]));
	ErrCheck(close(fd[1]));
}

void ChildTask(int fd[2])
{
	ErrCheck(close(fd[0]));
	ErrCheck(dup2(fd[1], STDOUT_FILENO));
	ErrCheck(close(fd[1]));

	char *const argv[] = {(char *)"/usr/bin/python3", (char *)"index.py", NULL};
	ErrCheck(execve("/usr/bin/python3", argv, NULL));
}

void ParentTask(int pid, int fd[2])
{
	int status;

	waitpid(pid, &status, WNOHANG);
	if (WIFEXITED(status))
		std::cout << "Child exited with status " << WEXITSTATUS(status) << std::endl;
	else
		std::cout << "Child exited with unknown status" << std::endl;

	ErrCheck(close(fd[1]));

	char buf[1024];
	int  res = ErrCheck(read(fd[0], buf, 1023));
	buf[res] = '\0';
	std::cout << buf << std::endl;
}
