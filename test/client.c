#include <unistd.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netdb.h>

#define PORT "80"
#define HOST "127.0.0.1"
#define OK 0

typedef struct sockaddr			t_sa;
typedef struct sockaddr_storage t_sa_storage;
typedef struct addrinfo			t_addrinfo;

int try_connect_socket(t_addrinfo *lst)
{
	int sock_fd;

	for (; lst; lst = lst->ai_next)
	{
		sock_fd = socket(lst->ai_family, lst->ai_socktype, lst->ai_protocol);
		if (sock_fd < 0)
			continue ;
		if (connect(sock_fd, lst->ai_addr, lst->ai_addrlen) == OK)
			return (sock_fd);
		close(sock_fd);
	}
	return (-1);
}

int create_socket(char *host, char *port, t_addrinfo *sock_config)
{
	int			sock_fd;
	t_addrinfo	*list;

	getaddrinfo(host, port, sock_config, &list);
	sock_fd = try_connect_socket(list);
	freeaddrinfo(list);
	return (sock_fd);
}

int open_client_fd(char *host, char *port)
{
	int			client_fd;
	t_addrinfo	config = {
		.ai_socktype = SOCK_STREAM,
		.ai_flags = AI_ADDRCONFIG | AI_NUMERICSERV,
	};
	t_addrinfo	*list;

	getaddrinfo(host, port, &config, &list);
	client_fd = try_connect_socket(list);
	freeaddrinfo(list);
	return (client_fd);
}

#define SIZE 1024

void run_client(int client_fd)
{
	char	buf[SIZE] = {};
	char	buf2[SIZE] = {};
	int		ret;

	while ((ret = read(STDIN_FILENO, buf, SIZE)) != 0)
	{
		send(client_fd, buf, ret, 0);
		ret = recv(client_fd, buf2, ret, 0);
		printf("recv : %d\n", ret);
		write(STDOUT_FILENO, "ret :", 5);
		write(STDOUT_FILENO, buf2, ret);
	}
}

int main()
{
	int client_fd = open_client_fd(HOST, PORT);
	run_client(client_fd);
}
