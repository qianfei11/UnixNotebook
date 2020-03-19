#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	pid_t pid;
	int ret;
	pid = fork(); // 创建新进程
	if (pid == 0) {
		printf("child proc: id = %d.\n", getpid()); // 输出子进程pid
		execl("/bin/ls", "ls", "-l", NULL); // 执行ls -l命令
        printf("exit child proc.\n");
	} else if (pid > 0) {
		printf("parent proc: id = %d.\n", getpid()); // 输出父进程pid
		waitpid(pid, &ret, 0); // 等待子进程结束
		printf("child proc finished.\n");
		printf("exit parent proc.\n");
	} else {
		printf("fork error.\n");
	}
	printf("parent proc.\n");
	return 0;
}
