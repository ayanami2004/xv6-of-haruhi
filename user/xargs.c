#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "kernel/fs.h"

#define MSGSIZE 36

int main(int argc, char *argv[])
{
    // 为了保证能读取到命令，进行短暂的暂停
    sleep(10);

    // 第一步：获取前一个命令的标准化输出
    char buf[MSGSIZE];
    read(0, buf, MSGSIZE);

    // 第二步：获取自己的命令行参数
    char *xargv[MSGSIZE];
    int xargc = 0;
    for (int i = 1; i < argc; i++)
    {
        xargv[xargc++] = argv[i];
    }

    char *p = buf;
    for (int i = 0; i < MSGSIZE; i++)
    {
        if (buf[i] == '\n')
        {
            // 创建子进程去执行命令
            int pid = fork();
            if (pid > 0)
            {
                p = &buf[i + 1]; // 指针移动到命令结束处
                wait(0);         // 等待子进程结束
            }
            else if (pid == 0)
            {
                // 第三步：在子进程中执行整个命令（使用exec）
                buf[i] = 0;         // 终止符
                xargv[xargc++] = p; // 跟入前一个命令标准化输出的内容
                xargv[xargc++] = 0; // 终止符
            }
        }
    }

    // 结束
    wait(0);
    exit(0);
}