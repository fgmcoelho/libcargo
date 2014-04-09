#ifndef __MODIFIED_HEAP_H_GUARD__
#define __MODIFIED_HEAP_H_GUARD__

#define HEAP_STARTING_SIZE 256

#include <stdlib.h>

struct modified_heap_st;

typedef struct modified_heap_st modifiedHeap;

modifiedHeap* modifiedHeapCreate(int (*)(void*, void*), unsigned (*)(void*), void (*)(void*, unsigned));
int modifiedHeapInsert(modifiedHeap*, void*);
void* modifiedHeapGetFirstElement(modifiedHeap*);
void* modifiedHeapPopFirstElement(modifiedHeap*);
void modifiedHeapUpdatedValue(modifiedHeap*, void*);
void modifiedHeapReset(modifiedHeap*);
void modifiedHeapClear(modifiedHeap**, void (*)(void*));

struct modified_heap_iterator_st{
	modifiedHeap* h;
	unsigned index;
};

typedef struct modified_heap_iterator_st modifiedHeapIterator;

void modifiedHeapIteratorStart(modifiedHeap* l, modifiedHeapIterator* it);
void* modifiedHeapIteratorGetFirstElement(modifiedHeapIterator*);
void* modifiedHeapIteratorGetLastElement(modifiedHeapIterator*);
void* modifiedHeapIteratorGetNextElement(modifiedHeapIterator*);
void* modifiedHeapIteratorGetCurrentElement(modifiedHeapIterator*);
void modifiedHeapIteratorReset(modifiedHeapIterator*);


#endif
