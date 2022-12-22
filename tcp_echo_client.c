#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

void handle(int sockfd);

// 读取一行字符串
int read_line(int sockfd, void *vptr, size_t len);

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("usage: tcp_echo_client <IP>\n");
		exit(1);
	}

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8888);
	// 将点分十进制的IP地址转为用于网络传输的数值格式
	const char *addr = argv[1];
	inet_pton(AF_INET, addr, &servaddr.sin_addr);

	// conn server socket
	int ret =
	    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	printf("connect ret:%d,servaddr.sin_addr=%s\n", ret,
	       inet_ntoa(servaddr.sin_addr));

	handle(sockfd);
	exit(0);
}

// 处理网络消息
void handle(int sockfd) {
	/* printf("handle,sockfd=%d\n", sockfd); */
	char sendline[1024], recvline[1024];

	// 从标准输入读取1024个字节存到sendline
	while (fgets(sendline, 1024, stdin) != NULL) {
		// block 发送数据到指定 sockfd
		write(sockfd, sendline, strlen(sendline));
		/* printf("handle,1,sockfd=%d\n", sockfd); */

		// 读取来自服务端的响应
		if (read_line(sockfd, recvline, 1024) == 0) {
			printf("handle error, server terminated prematurely");
			exit(1);
		}

		// 发送字符串到标准输出
		fputs(recvline, stdout);
	}
}

int read_line(int sockfd, void *vptr, size_t maxlen) {
	int n, rc;
	char c, *ptr;

	ptr = vptr;

	for (n = 1; n < maxlen; n++) {
		// read a single byte
		if ((rc = read(sockfd, &c, 1)) == 1) {
			// 正确读取到1个字符
			*ptr++ = c;
			if (c == '\n') {
				break;
			}
		} else if (rc == 0) {
			// 未读到字符
			// the file offset is at or past the end of file
			if (n == 1) {
				return 0;
			} else {
				break;
			}
		} else {
			return -1;
		}
	}

	// 回收
	*ptr = 0;
	return n;
}
