#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#include <math.h>
#include <string.h>

// long long fiboArry[100];

unsigned long long fibo(int n){
	return 1/sqrt(5) * (pow((1+sqrt(5))/2, n) - pow((1-sqrt(5))/2, n));
}

int main(int argc, char* argv[])
{
    int numprocs, myid, source;
    MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    if (myid != 0) {  //非0号进程发送消息
		 long long fiboArry[100];
		//long long test[100];
		int num = 0;
		MPI_Recv(&num, 1, MPI_INT, 0, 38, MPI_COMM_WORLD, &status);

		int startpos = num/(numprocs - 1) * (myid - 1);
		int endpos = (myid == (numprocs - 1))? num - 1 : startpos + num/(numprocs - 1) - 1;
		//starts with 0
		int len = endpos - startpos + 1;
		for(int i = startpos+1; i <= endpos+1; ++i){
			fiboArry[i] = fibo(i);
			//printf("In [rank %d], compute fibo: %d %llu\n", myid, i, fiboArry[i]);
			//MPI_Send(&fiboArry[i], sizeof(long long), MPI_LONG_LONG, 0, 38, MPI_COMM_WORLD);
		}
		MPI_Send(fiboArry+startpos+1, len, MPI_LONG_LONG, 0, 99, MPI_COMM_WORLD);
		//printf("In [rank %d], len : %d   array: %lld\n", myid, len, *(fiboArry+startpos+1));

		// int temp = 0;
		//temp = fibo(3);
		// temp = 3;
		// MPI_Send(&temp, 1, MPI_INT, 0, 99, MPI_COMM_WORLD);
    }
    else {   // myid == 0，即0号进程接收消息
		long long fiboArry[100];
		//int testmain[100];
		// long long fiboArryTemp[100];
		long long *pfibo = fiboArry;
		int num = 0;
		scanf("%d", &num);
		MPI_Send(&num, 1, MPI_INT, 1, 38, MPI_COMM_WORLD);
		MPI_Send(&num, 1, MPI_INT, 2, 38, MPI_COMM_WORLD);
		MPI_Send(&num, 1, MPI_INT, 3, 38, MPI_COMM_WORLD);
		for(int i = 1; i< numprocs; ++i){
			int received_count = 0;
			MPI_Probe(i, 99, MPI_COMM_WORLD, &status);
			// get real length before recv
			MPI_Get_count(&status, MPI_LONG_LONG, &received_count);
			//printf("in rank 0, received_count from %d: %d\n", i, received_count);
			// int temp = 0;
			// MPI_Recv(&temp, 1, MPI_INT, i, 99, MPI_COMM_WORLD, &status);
			MPI_Recv(pfibo, received_count, MPI_LONG_LONG, i, 99, MPI_COMM_WORLD, &status);
			//printf("from  %d   %lld  ->  %lld\n", i, *(pfibo), *(pfibo + received_count - 2));
			//startpos += received_count;
			pfibo += received_count;
		}

		printf("%d", 1);
		for(int i = 1; i<=num - 1; ++i){
			printf(" %lld", fiboArry[i]);
		}
		printf("\n");
    }
    MPI_Finalize();

	return 0;
}
