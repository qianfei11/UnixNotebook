#include <stdio.h>
#include <unistd.h>

int main() {
	printf("Hello %d!\n", getpid());
	return 0;
}
