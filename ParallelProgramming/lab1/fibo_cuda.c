#include <stdio.h>
#include <math.h>


/***** Begin *****/

__global__ void calFibo(long long *fiboArry)
{
    int tid = threadIdx.x;
	fiboArry[tid] = (long long)((pow((1+sqrt(5.0)), tid) - pow((1-sqrt(5.0)), tid))/( pow((double)2, tid) * (sqrt(5.0)) ) + 0.5);
}

int main()
{
    int num = 0;
    scanf("%d", &num);
	long long fiboResult[100];
	long long *fiboCal;
	cudaMalloc((void**)&fiboCal, sizeof(long long)*(num+1));
    calFibo<<<1,num+1>>>(fiboCal);
    cudaMemcpy(&fiboResult,fiboCal,sizeof(long long)*(num+1),cudaMemcpyDeviceToHost);
    printf("%d", 1);
    for(int i = 1; i< num; ++i){
        printf(" %lld", fiboResult[i]);
    }
    printf("\n");
    cudaFree(fiboCal);
	return 0;
}
/***** End *****/
