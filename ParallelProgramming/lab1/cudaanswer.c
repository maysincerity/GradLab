#include <stdio.h>
#include <math.h>
/*********Begin*********/
__global__ void fib(long long *c)
{
        int tid = threadIdx.x;
        double gh5=sqrt((double)5);
        c[tid] = (long long)((pow((1+gh5),tid)-pow((1-gh5),tid))/(pow((double)2,tid)*gh5)+0.5);
}
int main(void)
{
        int n = 0;
        scanf("%d", &n);
        int max_len = 60;
        long long host_c[max_len];
        long long *dev_c;
        cudaMalloc((void**)&dev_c, sizeof(long long)*max_len);
        fib<<<1,max_len>>>(dev_c);
        cudaMemcpy(&host_c, dev_c, sizeof(long long)*max_len, cudaMemcpyDeviceToHost);
        for(int i = 1; i < n; ++i)
        {
                printf("%lld ", host_c[i]);
        }
        if(n == 1)
        {
            printf("1\n");
        }
        else
        {
            printf("%lld\n", host_c[n]);
        }
        cudaFree(dev_c);
        return 0;
}
/*********End*********/
