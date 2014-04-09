#ifndef __POOL_H_GUARD__
#define __POOL_H_GUARD__

#include "list.h"

struct pool_st;

typedef struct pool_st pool;

pool* poolCreate(unsigned , unsigned );
void* poolGetElement(pool* );
int poolReturnElement(pool* , void* );
void poolClear(pool**);

#endif
