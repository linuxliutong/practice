#ifndef __FACTORY_H__
#define __FACTORY_H__

#include "product.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _Factory;
typedef struct _Factory Factory;

typedef Operation* (*CreateOperFunc)(char operName);
typedef void (*FactoryDestroyFunc)(Factory *thiz);

struct _Factory
{
    CreateOperFunc create_oper;
    FactoryDestroyFunc destroy;
};

Factory *FactoryCreate(void);
Operation* factory_create_oper(char operName);
void factory_destroy(Factory *thiz);

#ifdef __cplusplus
}
#endif

#endif


