#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>


/***** Begin *****/
long long fiboArry[100];

struct fiboArg{
	int startpos;
	int endpos;
};

unsigned long long fibo(int n){
	return 1/sqrt(5) * (pow((1+sqrt(5))/2, n) - pow((1-sqrt(5))/2, n));
}

void *calFibo(void *arg){
	int i = 0;
	int startpos=0,endpos=0;
	struct fiboArg *myarg;
	myarg = (struct fiboArg *)arg;
	startpos = myarg->startpos;
	endpos = myarg->endpos;
	for(i=startpos;i<endpos;++i){
		//printf(" %llu", fibo(i));
		fiboArry[i] = fibo(i);
	}
	return NULL;
}

int main()
{
	//define thread pid
	pthread_t threads[4];
	void *thread1_ret, *thread2_ret, *thread3_ret, *thread4_ret;


	int i = 0;
	int num = 0;
	int threadNum = 0;
	int perGroupNum = 0;
	struct fiboArg farg[50];
	scanf("%d", &num);
	//printf("%d\n", 1);
	fiboArry[0] = 0;
	fiboArry[1] = 1;
	perGroupNum = ceil((num - 1)/4.0);
	threadNum = 4 > perGroupNum ? perGroupNum : 4;
	if(threadNum < 4){
		perGroupNum = ceil((num - 1.0)/threadNum);
	}
	//printf("%d %d\n", perGroupNum, threadNum);

	//get thread's parameter
	for(i=0; i<threadNum; ++i){
		farg[i].startpos = i * perGroupNum + 2;
		farg[i].endpos = ((i+1) * perGroupNum + 2) >= num ? num+1 : ((i+1) * perGroupNum + 2);
		//printf("%d %d\n", farg[i].startpos, farg[i].endpos);
	}

	//create threads
	for(int index=0; index < threadNum; ++index){
		pthread_create(&threads[index], NULL, calFibo, (void *)&(farg[index]));
	}

	//wait
	for(int index=0; index < threadNum; ++index){
		void **returnVal;
		pthread_join(threads[index], NULL);
	}

	//show fibo
	printf("%d", 1);
	for(int index=2; index <= num; ++index){
		printf(" %lld", fiboArry[index]);
	}

	printf("\n");


	return 0;
}
/***** End *****/
