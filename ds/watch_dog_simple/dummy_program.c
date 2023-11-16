#include <stdio.h>

int main(int argc, char *argv[])
{
    int i = 0;

    printf("This program is called: \"%s\"\n", argv[0]);
    for(i = 1; i < argc; ++i)
    {
        printf("argv[%d] = %s\n", i, argv[i]);
    }

    return 0;
}