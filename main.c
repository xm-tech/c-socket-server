#include "socket_server.h"
#include <signal.h>

void _poll(){
}

void test_pointer_subscript() {
	struct player {
		char * name;
		int age;
	} *p;
	p->name="maxm";	
	p->age=30;

	int i=0;
	
	for (int i=0; i<1; i++) {
		struct player pi = p[i];
	}
}

int main(){
	struct socket_server *ss = socket_server_create(0);
	pthread_t pid;
	return 0;
}
