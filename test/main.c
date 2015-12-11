#include <stdio.h>
#include "test.h"

int main(void)
{
    printf("[%s, %d] flag : %d\n", __FUNCTION__, __LINE__, flag);
    fun();
    if(flag == 1)
    {
        printf("NICE\n");
    }
    printf("[%s, %d] flag : %d\n", __FUNCTION__, __LINE__, flag);
    return 0;
}

