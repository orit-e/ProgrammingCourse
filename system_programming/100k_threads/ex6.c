#include <stdio.h>      /*  printf(), size_t    */
#include <omp.h>        /*  OpenMP              */

#define INTEGER_TO_SOD 1234567890

int main()
{
    size_t i = 0;
    size_t sod_tot = 0;

    #pragma omp parallel for
    for(i = 1; i <= INTEGER_TO_SOD; ++i)
    {
        sod_tot += (0 == INTEGER_TO_SOD % i) ? i : 0;
    }

    printf("Sume of divisors of %u is: %lu\n", INTEGER_TO_SOD, sod_tot);

    return 0;
}
