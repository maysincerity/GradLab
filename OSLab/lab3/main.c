#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "my_struct.h"

int buf_empty;//空闲缓冲区
int buf_full;//满缓冲区

void P(int semid,int index)
{
    struct sembuf sem;
    sem.sem_num = index;
    sem.sem_op = -1;
    sem.sem_flg = 0;
    semop(semid,&sem,1);    
    return;
}

void V(int semid,int index)
{
    struct sembuf sem;
    sem.sem_num = index;
    sem.sem_op =  1;
    sem.sem_flg = 0;
    semop(semid,&sem,1);
    return;
}

int main() {
    int status;//子进程状态
    pid_t writebuf_pid;
    pid_t readbuf_pid;
    int shm_id[BUF_SIZE];//共享内存
    sh_mem *p;

    //产生信号灯
    buf_empty = semget(1,1,IPC_CREAT|0666);
    buf_full  = semget(2,1,IPC_CREAT|0666);

    //赋值
    union semun arg1;
    union semun arg2;
    arg1.val=10;
    arg2.val=0;
    semctl(buf_empty,0,SETVAL,arg1);
    semctl(buf_full,0,SETVAL,arg2);

    //产生共享内存区
    for(int i=0;i<BUF_SIZE;++i)
    {
        shm_id[i] = shmget(s_addr+i,sizeof(sh_mem),IPC_CREAT|0666);
    }

    printf("Copy ... \n");

    while ((writebuf_pid = fork()) == -1) ;
    if (writebuf_pid == 0) {//子进程
        execlp("./writebuf", "writebuf", NULL);
    } else {//main
        while ((readbuf_pid = fork()) == -1);
        if (readbuf_pid == 0) {//子进程
            execlp("./readbuf", "readbuf", NULL);
        } else {// main
            //等待子进程结束
            for (int i = 0; i < 2; i++) {
                waitpid(-1, &status, 0);
            }
            printf("Done.\n");

            //删除共享内存区
            for(int i=0;i<BUF_SIZE;++i)
            {
                shmctl(shm_id[i],IPC_RMID,0);
            }

            //删除信号灯
            semctl(buf_empty,0,IPC_RMID);
            semctl(buf_full,0,IPC_RMID);

            return 0;
        }
    }
}
