#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <pthread.h>

int semid,a=0;
int add_num=1;

union semun {
    int              val;    // value for SETVAL
    struct semid_ds *buf;    // buffer for IPC_STAT, IPC_SET
    unsigned short  *array;  // array for GETALL, SETALL
    struct seminfo  *__buf;  // buffer for IPC_INFO (Linux-specific)
};

void P(int semid, int index)
{      
    struct sembuf sem;
    sem.sem_num = index;/*要操作的信号灯的编号*/
    sem.sem_op = -1; /*要执行的操作*/
    sem.sem_flg = 0; /*操作标志，一般设置为0*/
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

void *thread1(void *arg)
{
    for(int i=0;i<100;i++)
    {
        P(semid,0);
        a+=add_num;
        add_num++;
        V(semid,1);
    }
}

void *thread2(void *arg)
{
    for(int i=0;i<100;i++)
    {
        P(semid,1);
        printf("a=%d\n",a);
        V(semid,0);
    }
}

int main()
{
    pthread_t pid1,pid2;
    int ret;
    union semun arg1;
    union semun arg2;

    //创建信号灯
    semid=semget(IPC_PRIVATE,2,IPC_CREAT|0666);
    arg1.val=1;
    arg2.val=0;

    //信号灯赋值
    semctl(semid,0,SETVAL,arg1);//empty buffer
    semctl(semid,1,SETVAL,arg2);//full buffer

    //创建两个线程
    while((ret=pthread_create(&pid1,NULL,thread1,NULL))!=0);
    while((ret=pthread_create(&pid2,NULL,thread2,NULL))!=0);

    //等待线程结束
    pthread_join(pid1,NULL);
    pthread_join(pid2,NULL);

    //删除信号灯集
    semctl(semid, 0, IPC_RMID);

}