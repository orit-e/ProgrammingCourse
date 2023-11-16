#include <stdio.h>

void InitCountLUT(size_t *count_LUT)
{
    size_t i = 0;

    for(i = 0; 256 > i; ++i)
    {
        unsigned char result = ((unsigned char)i & 0x55) + 
                                (((unsigned char)i >> 1) & 0x55);
	    result = (result & 0x33) + ((result >> 2) & 0x33);
        result = (result & 0x0f) + ((result >> 4) & 0x0f);
        count_LUT[i] = (size_t)result;
    }
}

void InitStringLUT(char string_LUT[256][9])
{
    size_t i = 0;
    size_t j = 0;

    for(i = 0; 256 > i; ++i)
    {
        for(j = 0; 8 > j; ++j)
        {
            string_LUT[i][7 - j] = '0' + ((i >> j) & 1l);
        }
        string_LUT[i][8] = '\0';
    }
}


int main()
{
    size_t count_LUT[256] = {0};
    char string_LUT[256][9] = {0};
    size_t i = 0;

    InitCountLUT(count_LUT);
    InitStringLUT(string_LUT);

    for(i = 0; 256 > i; ++i)
    {
        printf("%lu, ", count_LUT[i]);
    }
    printf("\n\n");
    for(i = 0; 256 > i; ++i)
    {
        printf("\"%s\", ", string_LUT[i]);
    }
    printf("\n\n");

    return 0;

}