#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _Ret
{
    RET_OK,
    RET_FAIL
}Ret;

#define return_if_fail(p)\
    if(!(p)){\
    printf("%s:%d Warning:"#p"Failed\n",__func__,__LINE__);\
    return;}
#define return_val_if_fail(p, ret)\
    if(!(p)){\
    printf("%s:%d Warning:"#p"Failed\n",__func__,__LINE__);\
    return (ret);}
#define SAFE_FREE(p) if(p != NULL){free(p); p = NULL;}

#ifdef __cplusplus
}
#endif

#endif
