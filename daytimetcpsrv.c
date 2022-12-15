#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <strings.h>
#include <stdbool.h>


#define BACKLOG 100 /* sizeof(synrcved list) + sizeof(established list)*/
#define SA struct sockaddr
#define LISTEN_PORT 8888

int main(int argc, char **argv) {
	socklen_t len;
	struct sockaddr_in serveraddr, cliaddr;
	char buff[1024];
	time_t ticks;

	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(LISTEN_PORT);

	bool binded = bind(listenfd, (SA *)&serveraddr, sizeof(serveraddr));
	printf("binded:%d\n", binded);
	
	listen(listenfd, BACKLOG);
	
	while (true) {
		len = sizeof(cliaddr);
		int connfd = accept(listenfd, (SA *) &cliaddr, &len);
		/* connfd = accept(listenfd, NULL, NULL); */
		const char* caddr = inet_ntop(AF_INET,  &cliaddr.sin_addr, buff, sizeof(buff));
		int cport = ntohs(cliaddr.sin_port);
		printf("conn from %s, port: %d, connfd: %d", caddr, cport, connfd);
		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		write(connfd, buff, strlen(buff));
		close(connfd);
	}
}
