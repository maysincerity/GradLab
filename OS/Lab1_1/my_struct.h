#ifndef MYSTRUCT_H
#define MYSTRUCT_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#define DATA_SIZE 100000
#define BUF_SIZE 10 //共享内存数量
#define s_addr 100 

union semun {
    int              val;
    struct semid_ds *buf;
    unsigned short  *array;
    struct seminfo  *__buf;
};

typedef struct sh_mem
{
    char data[DATA_SIZE];
    int  length;
}sh_mem;

#endif
