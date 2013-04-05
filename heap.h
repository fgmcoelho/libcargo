#ifndef __HEAP_GUARD_H_
#define __HEAP_GUARD_H_

#define HEAP_STARTING_SIZE 256

#include <stdlib.h>

struct heap_t{
	void** info;
	int (*compareFunction)(void*, void*);
	unsigned used;
	unsigned currentSize;
};

typedef struct heap_t heap;

heap* heapCreate(int (*)(void*, void*));
int heapInsert(heap*, void*);
int heapRemove(heap*, void*);


#endif
