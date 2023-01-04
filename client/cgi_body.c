#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int open_clientfd(char *hostname, char *port)
{
	int             clientfd, rc;
	struct addrinfo hints, *listp, *p;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags    = AI_NUMERICSERV;
	hints.ai_flags |= AI_ADDRCONFIG;
	if ((rc = getaddrinfo(hostname, port, &hints, &listp)) != 0) {
		fprintf(stderr, "getaddrinfo failed (%s:%s): %s\n", hostname, port, gai_strerror(rc));
		return -2;
	}

	for (p = listp; p; p = p->ai_next) {
		if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
			continue;
		if (connect(clientfd, p->ai_addr, p->ai_addrlen) != -1)
			break;
		if (close(clientfd) < 0) {
			fprintf(stderr, "open_clientfd: close failed: %s\n", strerror(errno));
			return -1;
		}
	}

	freeaddrinfo(listp);
	if (!p)
		return -1;
	else
		return clientfd;
}
int main(int argc, char **argv)
{
	int   clientfd;
	char *host, *port;

	if (argc != 4) {
		fprintf(stderr, "usage: %s <host> <port> <body-size>\n", argv[0]);
		exit(0);
	}
	if (signal(SIGPIPE, SIG_IGN) < 0) {
		perror("sigpipe ign");
		exit(1);
	}

	host     = argv[1];
	port     = argv[2];
	clientfd = open_clientfd(host, port);
	if (clientfd == -1) {
		exit(1);
	}
	char   req[1000000] = {};
	size_t body_len     = atol(argv[3]);
	sprintf(
		req,
		"GET /cat.cgi HTTP/1.1\r\n"
		"host:a\r\n"
		"content-length: %zu\r\n"
		"\r\n",
		body_len
	);
	write(clientfd, req, strlen(req));
	for (size_t i = 0; i < body_len; i++) {
		if (write(clientfd, &"0123456789"[i % 10], 1) < 0) {
			break;
		}
	}
	while (1) {
		char    res[1000000] = {};
		ssize_t size         = read(clientfd, res, sizeof(res));
		if (size == 0) {
			break;
		}
		(void)res;
		printf("%s\n", res);
	}
	close(clientfd);
	exit(0);
}
