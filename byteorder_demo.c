#include <stdio.h>
#include <unp.h>

union data {
	short s;
	char c[sizeof(short)];
} un;


// TODO make main a snippet in vim
int main(int argc, char **argv) {
	printf("%s\n", CPU_VENDOR_OS);

	const unsigned long ss = sizeof(short);

	printf("un.c[0]=%d,un.c[1]=%d\n", un.c[0], un.c[1]);
	un.s = 0x0102;
	// FIXME why un.c here equals to 0x0102 ?
	printf("un.c[0]=%d,un.c[1]=%d\n", un.c[0], un.c[1]);

	if (ss == 2) {
		if (un.c[0] == 1 && un.c[1] == 2) {
			printf("big-endian\n");
		} else if (un.c[0] == 2 && un.c[1] == 1) {
			printf("little-endian\n");
		} else {
			printf("unknown\n");
		}
	} else {
		printf("sizeof(short)=%lu\n", ss);
	}

	return 0;
}
