#include <stdio.h>
#include <math.h>

/***** Begin *****/
long long fiboArry[100];

unsigned long long fibo(int n){
	return 1/sqrt(5) * (pow((1+sqrt(5))/2, n) - pow((1-sqrt(5))/2, n));
}

int main()
{
    int i;
    int n;
    // long long temp1 = 1;
    // long long temp2 = 1;
    // long long next_fibo;

    scanf("%d", &n);
    fiboArry[0] = 0;
    fiboArry[1] = 1;

    #pragma omp parallel for
    for(i = 2; i <= n; ++i){
        fiboArry[i] = fibo(i);
    }

    // printf("%d", 1);
    for(i = 1;i<=n;++i){
        printf("%llu ", fiboArry[i]);
    }
    printf("\n");
    return 0;
}
/***** End *****/
