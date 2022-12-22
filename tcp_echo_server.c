#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define BACKLOG 100 /* sizeof(synrcved list) + sizeof(established list)*/
#define SA struct sockaddr
#define LISTEN_PORT 8888

// 处理某个客户端连接
void handle_conn(int connfd);

int main(int argc, char **argv) {
	int listenfd;
	// the pid of forked sub progress
	pid_t childpid;
	socklen_t chillen;
	struct sockaddr_in cliaddr, serveraddr;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	// erases the data in the n bytes of the memory starting at the location
	// pointed to by serveraddr
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	// htonl : 主机子节序转换为网络子节序的 int32
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(LISTEN_PORT);

	int bind_ret = bind(listenfd, (SA *)&serveraddr, sizeof(serveraddr));
	printf("bind_ret:%n\n", &bind_ret);

	listen(listenfd, BACKLOG);

	for (;;) {
		int connfd = accept(listenfd, (SA *)&cliaddr, &chillen);

		// fork one sub progress to handle the business logic, and then
		// ref(listenfd)+=1, ref(connfd)+=1, and fork always return 0 in
		// the sub progress if succ
		childpid = fork();
		if (childpid == 0) {
			// listenfd 的引用计数值减 1
			close(listenfd);

			handle_conn(connfd);

			exit(0);
		}
		// connfd 的引用计数值减 1
		close(connfd);
	}

	return 0;
}

// 不断从连接上读取数据，并返回给客户端
void handle_conn(int connfd) {
	for (;;) {
		char buf[1024];
		// read system call
		int n = read(connfd, buf, 1024);
		if (n > 0) {
			// 正常读取到数据, 原样下发到客户端
			write(connfd, buf, n);
			/**
			 *
			 * Another method to specialize the printf function is
			 * to pass the number of characters in the string inside
			 * the %s specifier. One way of doing this is to
			 * statically hard-code the length of the string with an
			 * integer between the symbols % and s, or it can be
			 * replaced with * symbol to take another integer
			 * argument from the printf parameters. Note that both
			 * methods include . character before the number or
			 * asterisk in the specifier.
			 */
			printf("handle_conn, input:%.*s\n", n, buf);
			/* printf("handle_conn, input:%.6s\n", buf); */
		} else {
			if (errno == EINTR) {
				// 若错误是因为系统终端所致，则需要继续读取数据
				continue;
			} else {
				// 否则其他错误的话, 退出
				printf(
				    "handle_conn fail, connfd=%d, errno=%d\n",
				    connfd, errno);
				return;
			}
		}
	}
}
