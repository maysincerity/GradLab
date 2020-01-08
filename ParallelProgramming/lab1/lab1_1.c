#include <stdio.h>

/***** Begin *****/
int main()
{
    int i;
    int n;
    long long temp1 = 1;
    long long temp2 = 1;
    long long next_fibo;

    scanf("%d", &n);

    for (i = 1; i <= n; i++)
    {
        if(i == 1){
            printf("%lld", temp1);
        }
        else{
            printf(" %lld", temp1);
        }
        next_fibo = temp1 + temp2;
        temp1 = temp2;
        temp2 = next_fibo;
    }
    printf("\n");
    return 0;
}
/***** End *****/
