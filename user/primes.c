#include "kernel/types.h"
#include "user/user.h"
#define MAXNUM 36
void Prime(int rNum, int wNum)
{
    char buf[MAXNUM];
    int val = MAXNUM + 1;
    read(rNum, buf, MAXNUM);

    for (int i = 0; i < MAXNUM; i++)
    {
        if (buf[i] == 1)
        {
            val = i;
            break;
        }
    }
    if (val == MAXNUM + 1)
        exit(0);

    printf("prime %d\n", val);
    buf[val] = 0;
    for (int i = 0; i < MAXNUM; i++)
    {
        if (i % val == 0)
        {
            buf[i] = 0;
        }
    }

    int pid = fork();
    if (pid > 0)
    {
        write(wNum, buf, MAXNUM);
        wait(0);
    }
    else if (pid == 0)
    {
        Prime(rNum, wNum);
    }
}
int main()
{
    char IsPrime[MAXNUM];

    int fd[2];
    pipe(fd);

    int pid = fork();
    if (pid > 0) // 父进程
    {
        IsPrime[0] = 0;
        IsPrime[1] = 0;
        for (int i = 2; i < MAXNUM; i++)
        {
            IsPrime[i] = 1;
        }

        write(fd[1], IsPrime, MAXNUM);
        wait(0);
    }
    else if (pid == 0)
    {
        Prime(fd[0], fd[1]);
        wait(0);
    }

    exit(0);
}