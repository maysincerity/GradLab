
#include "mpi.h"
#include <stdio.h>
#include <math.h>
#define SIZE 10000

int isPrime(int num){
    if(num<2){
        return 0;
    }else if(num==2 || num==3){
        return 1;
    }else{
        int primeFlag=1;
        for(int i=2;i<(int)sqrt(num)+1;i++){
            if(num%i==0){
                primeFlag=0;
                break;
            }
        }
        return primeFlag;
    }
}

int main(int argc,char* argv[]) {
    int number[SIZE];
    MPI_Status status;
    FILE *fp=fopen("data.txt","r");
    if(fp==NULL) {
        printf("can not open data.txt!\n");
    } else {
        int totalNum=0;
        while(!feof(fp)) {
            if(totalNum<SIZE-1) {
                int num=0;
                fscanf(fp,"%d",&num);
                num?number[totalNum++]=num:0;
            } else {
                break;
            }
        }
        int rank,size;
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        if(rank==0){
            int result=0;
            int total=0;
            printf("in rank 0");
            for(int i=1;i<size;i++){
                MPI_Recv(&result,1,MPI_INT,i,0,MPI_COMM_WORLD, &status);
                total+=result;
            }
            printf("total number:%d\n",total);
        }else{
            int start=totalNum/(size-1)*(rank-1);
            int tail=(rank==(size-1))?totalNum-1:start+totalNum/(size-1)-1;
            int mySize=0;
            for(int i=start;i<=tail;i++){
                printf("[rank %d] judge number:%d\n",rank,number[i]);
                mySize=isPrime(number[i])?mySize+1:mySize;
            }
            printf("[rank %d]total prime number:%d\n",rank,mySize);
            MPI_Send(&mySize,1,MPI_INT,0,0,MPI_COMM_WORLD);
        }
        MPI_Finalize();
    }
    return 0;
}
