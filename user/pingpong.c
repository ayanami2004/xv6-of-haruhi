// #include <stdio.h>
// #include <unistd.h>
// #include <stdlib.h>
#include "kernel/types.h"
#include "user/user.h"

#define MSGSIZE 16

int main()
{
    int fd[2];
    char buf[MSGSIZE]; // 用于接收
    pipe(fd);

    int pid = fork();
    if (pid > 0) // 父进程
    {
        write(fd[1], "ping\n", MSGSIZE);
        read(fd[0], buf, MSGSIZE);
        printf("F received：%s", buf);
    }
    else
    {
        read(fd[0], buf, MSGSIZE);
        printf("S received：%s", buf);
        write(fd[1], "pong\n", MSGSIZE);
    }
    exit(0);
}