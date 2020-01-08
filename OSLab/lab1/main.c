#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t p1, p2;        //子进程
int pipe_fd[2];     //管道

void my_func(int sig_no) {
    if(sig_no==SIGINT){//向p1、p2发消息
        kill(p1, SIGUSR1);
        kill(p2, SIGUSR2);
    }
    if(sig_no==SIGUSR1){
        close(pipe_fd[1]);//关闭写
        printf("\nChild Process 1 is Killed by Parent!");
        exit(0);
    }
    if(sig_no==SIGUSR2){
        close(pipe_fd[0]);//关闭读
        printf("\nChild Process 2 is Killed by Parent!");
        exit(0);
    }
}

int main(void) {
    char writebuf[100];     //写缓冲区
    char readbuf[100];     //读缓冲区
    int status;         //子进程状态信息，用于等待结束

    if (pipe(pipe_fd) < 0) {
        printf("Creat pipe fail.\n");
        exit(-1);
    }

    signal(SIGINT, my_func);//设置软中断信号

    while ((p1 = fork()) == -1) ;
    if (p1 == 0) {                      
        //子进程 1
        int count = 1;
        signal(SIGINT, SIG_IGN);        //忽略SIGINT
        signal(SIGUSR1, my_func);       //处理子进程1
        //写数据
        close(pipe_fd[0]);
        while (1) {
            sprintf(writebuf, "I send you %d times\n", count++);
            if (write(pipe_fd[1], writebuf, 100) < 0) {
                printf("Fail to write.\n");
                exit(-1);
            }
            sleep(1);
        }
    } 
    else {
        //父进程执行
        while ((p2 = fork()) == -1) ;
        if (p2 == 0) {
            //子进程2
            signal(SIGINT, SIG_IGN);    //忽略SIGINT
            signal(SIGUSR2, my_func);   //处理子进程2
            //读数据
            close(pipe_fd[1]);
            while (1) {
                if (read(pipe_fd[0], readbuf, 100) < 0) {
                    printf("Fail to read.\n");
                    exit(-1);
                }
                printf("%s",readbuf);
            }
        } 
        else {                        
            //父进程执行
            for (int i = 0; i < 2;i++) {
                waitpid(-1, &status, 0);//等待两个子进程
            }
            //关闭管道
            close(pipe_fd[0]);
            close(pipe_fd[1]);
            printf("\nParent Process is Killed!\n");
        }
    }

    return 0;
}
