#include <unistd.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/select.h>

#define PORT 80
#define LISTENQ 1024

int	ft_utoa_len(uint32_t n, char *s, int len)
{
	if (n > 9)
		len = ft_utoa_len(n / 10, s, len);
	s[len++] = "0123456789"[n % 10];
	return (len);
}

unsigned long	ft_labs(long n)
{
	if (n < 0)
		return (-(n + 1) + 1UL);
	else
		return (n);
}

void	ft_itoa_buf(int n, char *buf)
{
	uint32_t	u;
	size_t		len;

	u = ft_labs(n);
	if (n < 0)
		buf[0] = '-';
	len = ft_utoa_len(u, buf, n < 0);
	buf[len] = '\0';
}

typedef struct sockaddr			t_sa;
typedef struct sockaddr_storage t_sa_storage;
typedef struct addrinfo			t_addrinfo;

# define GREEN "\x1b[32;1m"
# define RESET "\x1b[39;0m"

#define STR GREEN"\nConnection Success!!\n\n"RESET
#define OK 0
#define SIZE 1024
void serve_content(int connfd)
{
	char buf[SIZE] = {};

	int ret = recv(connfd, buf, SIZE, 0);
	send(connfd, buf, ret, 0);
	// send(connfd, STR, sizeof(STR), 0);
}


void echo_stdin()
{
	char *res = NULL;
	size_t size = 0;

	getline(&res, &size, stdin);
	printf("%s", res);
	free(res);
}

void run_event_loop(int listen_fd)
{
	int				connfd;
	t_sa_storage	client_addr;
	socklen_t		client_len = sizeof(t_sa_storage);
	fd_set			read_set;
	fd_set			ready_set;

	FD_ZERO(&read_set);
	FD_SET(STDIN_FILENO, &read_set);
	FD_SET(listen_fd, &read_set);
	while (true)
	{
		ready_set = read_set;
		select(listen_fd + 1, &ready_set, NULL, NULL, NULL);
		if (FD_ISSET(STDIN_FILENO, &ready_set))
			echo_stdin();
		if (FD_ISSET(listen_fd, &ready_set))
		{
			connfd = accept(listen_fd, (t_sa *)&client_addr, &client_len);
			serve_content(connfd);
			close(connfd);
		}
	}
}

int try_bind_socket(t_addrinfo *lst)
{
	int sock_fd;
	int	optval = 1;

	for (; lst; lst = lst->ai_next)
	{
		sock_fd = socket(lst->ai_family, lst->ai_socktype, lst->ai_protocol);
		if (sock_fd < 0)
			continue ;
		setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
		if (bind(sock_fd, lst->ai_addr, lst->ai_addrlen) == OK)
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
	sock_fd = try_bind_socket(list);
	freeaddrinfo(list);
	return (sock_fd);
}

int open_listen_fd(int port)
{
	int			listen_fd;
	char		portstr[50];
	t_addrinfo	config = {
		.ai_socktype = SOCK_STREAM,
		.ai_flags = AI_PASSIVE | AI_ADDRCONFIG | AI_NUMERICSERV,
	};

	ft_itoa_buf(port, portstr);
	listen_fd = create_socket(NULL, portstr, &config);
	if (listen(listen_fd, LISTENQ) == -1)
	{
		perror("");
		exit(1);
	}
	return listen_fd;
}

int main()
{
	int listen_port = PORT;
	int	listen_fd;

	listen_fd = open_listen_fd(listen_port);
	run_event_loop(listen_fd);
}
