#ifndef __HEAP_H_GUARD__
#define __HEAP_H_GUARD__

#define HEAP_STARTING_SIZE 256

#include <stdlib.h>

struct heap_st;

typedef struct heap_st heap;

heap* heapCreate(int (*)(void*, void*));
int heapInsert(heap*, void*);
void* heapGetFirstElement(heap*);
void* heapPopFirstElement(heap*);
unsigned heapGetSize(heap*);
void heapClear(heap**, void (*)(void*));

#endif
