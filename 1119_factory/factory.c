#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "factory.h"
#include "producta.h"
#include "productb.h"

Operation* factory_create_oper(char operName)
{
    Operation *thiz = malloc(sizeof(Operation));

    if(thiz != NULL)
    {
        switch(operName)
        {
            case '+':
                thiz = (Operation *)oper_add_create();
                break;
            case '-':
                thiz = (Operation *)oper_minus_create();
                break;
            default:;
        }
    }

    return thiz;
}

void factory_destroy(Factory *thiz)
{
    if(thiz	!= NULL)
    {
        SAFE_FREE(thiz);
    }

    return ;
}

Factory *FactoryCreate(void)
{
    Factory *thiz = malloc(sizeof(Factory));
    printf("sizeof(Factory) : %d\n", sizeof(Factory));
    if(thiz != NULL)
    {
        thiz->create_oper = factory_create_oper;
        thiz->destroy = factory_destroy;
    }
}

