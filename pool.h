#ifndef _POOL_H_GUARD__
#define _POOL_H_GUARD__

#include "list.h"

struct pool_t {
	unsigned sizeOfElements, elementsPerBlock;
	list *freeRefs, *memoryBlocks;;
};

typedef struct pool_t pool;

pool* poolCreate(unsigned , unsigned );
void* poolGetElement(pool* );
int poolReturnElement(pool* , void* );
void poolClear(pool*);

#endif
