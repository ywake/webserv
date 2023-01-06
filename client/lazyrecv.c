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

#define DEFAULT_BODY_LEN 1024 * 1024

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

void print_response(int clientfd)
{
	while (1) {
		char    res[1000000] = {};
		ssize_t size         = read(clientfd, res, sizeof(res));
		printf("%zd\n", size);
		if (size <= 0) {
			break;
		}
		printf("%s\n", res);
	}
}

int main(int argc, char **argv)
{
	int   clientfd;
	char *host, *port;

	if (argc != 3) {
		fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
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
	int flag = fcntl(clientfd, F_GETFL, 0);
	if (flag == -1) {
		perror("fcntl");
		exit(1);
	}
	if (fcntl(clientfd, F_SETFL, flag | O_NONBLOCK) == -1) {
		perror("fcntl");
		exit(1);
	}
	char req[1000000] = {};
	sprintf(
		req,
		"GET /inf0.cgi HTTP/1.1\r\n"
		"host:a\r\n"
		"\r\n"
	);
	write(clientfd, req, strlen(req));
	for (int i = 0; i < 10; i++) {
		printf("req %d\n", i);
		ssize_t size = sprintf(
			req,
			"GET / HTTP/1.1\r\n"
			"host:a\r\n"
			"\r\n"
		);
		size_t idx = 0;
		int    end = 0;
		while (1) {
			ssize_t wc = write(clientfd, req, size);
			printf("%zd %zd\n", size, wc);
			if (size == wc) {
				break;
			} else if (wc < 0) {
				perror("write");
				end = 1;
				break;
			} else {
				size -= wc;
				idx += wc;
			}
		}
		if (end) {
			break;
		}
	}
	sleep(35);
	print_response(clientfd);
	close(clientfd);
}
