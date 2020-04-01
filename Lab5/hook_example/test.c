#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Usage: %s [RUID] [EUID]\n", argv[0]);
		return -1;
	}
	int buf;
	uid_t ruid, euid;
	buf = atoi(argv[1]);
	if (buf == 0) {
		printf("Argument illegal.\n");
		return -1;
	}
	ruid = (uid_t) buf;
	buf = atoi(argv[2]);
	if (buf == 0) {
		printf("Argument illegal.\n");
		return -1;
	}
	euid = (uid_t) buf;
	int ret = setreuid(ruid, euid);
	if (ret != 0) {
		printf("SYS_setreuid exited for some reason.\n");
		return -1;
	}
	system("/bin/sh");
	return 0;
}
