#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include<fcntl.h>
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
    int fp;
    int shm_id[BUF_SIZE];
    sh_mem *p;

    //获取信号灯
    buf_empty = semget(1,1,IPC_CREAT|0666);
    buf_full  = semget(2,1,IPC_CREAT|0666);

    //获取共享内存区
    for(int i=0;i<BUF_SIZE;++i)
    {
        shm_id[i] = shmget(s_addr+i,sizeof(sh_mem),IPC_CREAT|0666);
    }

    //打开文件
    if((fp = open("dst",O_WRONLY|O_CREAT,0666)) == -1)
    {
        printf("Fail to open file.\n");
    }

    int i=0,temp=0;
    while (1) {
        P(buf_full,0);
        p=(sh_mem *)shmat(shm_id[i],0,0);
        write(fp,p->data,p->length);
        V(buf_empty,0);
        temp=p->length;//判断结束条件
        shmdt(p);
        i=(i+1)%BUF_SIZE;//环形缓冲区
        if(temp<DATA_SIZE) break;
    }

    //关闭文件
    close(fp);

    return 0;
}
