#include <stdio.h>      /*  printf(), size_t                */
#include <pthread.h>    /*  pthread_create(), pthread_t     */
#include <unistd.h>     /*  sleep()                         */

#define SIZE 6
#define INTEGER_TO_SOD 1234567890

typedef struct sod
{
        size_t integer;
        size_t range_min;
        size_t range_max;
}sod_t;

sod_t sod[SIZE];

void *SumOfDivisors(void *ptr);

int main()
{
    pthread_t thread_id[SIZE] = {0};
    size_t i = 0;
    size_t sod_tot = 0;

    for(i = 0; i < SIZE; ++i)
    {
        sod[i].integer = INTEGER_TO_SOD;
        sod[i].range_min = ((INTEGER_TO_SOD / SIZE) + 1) * i + 1;
        sod[i].range_max = ((INTEGER_TO_SOD / SIZE) + 1) * (i + 1);
        pthread_create(&thread_id[i], NULL, &SumOfDivisors, &sod[i]);
    }
        
    for(i = 0; i < SIZE; ++i)
    {
        size_t sod_tmp = 0;
        pthread_join(thread_id[i], (void **)&sod_tmp);
        sod_tot += sod_tmp;
    }
    printf("Sume of divisors of %u is: %lu\n", INTEGER_TO_SOD, sod_tot);
    printf("Num of threads = %u\n", SIZE);

    return 0;
}

void *SumOfDivisors(void *ptr)
{
    sod_t *sod = (sod_t *)ptr;
    size_t i = 0;
    size_t counter = 0;

    for(i = sod->range_min; i <= sod->range_max; ++i)
    {
        counter += (0 == sod->integer % i) ? i : 0;
    }

    return (void *)counter;
}


