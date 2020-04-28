#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

int main() {
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    printf("tv.tv_sec => %d\n", tv.tv_sec);
    printf("tv.tv_usec => %d\n", tv.tv_usec);
    return 0;
}
