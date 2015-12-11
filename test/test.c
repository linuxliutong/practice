#include <stdio.h>
#include "test.h"

int fun()
{
    printf("[%s, %d] flag : %d\n", __FUNCTION__, __LINE__, flag);
    flag = 1;
    printf("[%s, %d] flag : %d\n", __FUNCTION__, __LINE__, flag);
    return 0;
}
