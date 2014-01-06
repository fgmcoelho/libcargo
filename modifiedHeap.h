#ifndef __HEAP_GUARD_H_
#define __HEAP_GUARD_H_

#define HEAP_STARTING_SIZE 256

#include <stdlib.h>

struct modified_heap_t{
	void** info;
	int (*compareFunction)(void*, void*);
	unsigned (*getIndexFunction)(void*);
	void (*setIndexFunction)(void*, unsigned);
	unsigned used;
	unsigned currentSize;
};

typedef struct modified_heap_t modifiedHeap;

modifiedHeap* modifiedHeapCreate(int (*)(void*, void*), unsigned (*)(void*), void (*)(void*, unsigned));
int modifiedHeapInsert(modifiedHeap*, void*);
void* modifiedHeapGetFirstElement(modifiedHeap*);
void* modifiedHeapPopFirstElement(modifiedHeap*);
void modifiedHeapUpdatedValue(modifiedHeap*, void*);
void modifiedHeapReset(modifiedHeap*);
void modifiedHeapClear(modifiedHeap*, void (*)(void*));

#endif
