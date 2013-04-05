#include "heap.h"

heap* heapCreate(int (*compareFunction)(void*, void*)){

	heap* newHeap = (heap*)malloc(sizeof(heap));

	if (newHeap == NULL){
		return NULL;
	}
	
	newHeap->info = (void**) malloc(HEAP_STARTING_SIZE*sizeof(void*));
	if (newHeap->info == NULL){
		free(newHeap);
		return NULL;
	}

	newHeap->used = 0;
	newHeap->currentSize = HEAP_STARTING_SIZE;
	newHeap->compareFunction = compareFunction;

	return newHeap;
}

int heapInsert(heap* h, void* data){

	if (h == NULL){
		return 0;
	}

	if (h->used == 0){
		h->info[0] = data;
		h->used = 1;
		return 1;
	}

	h->info[h->used] = data;
	
	unsigned index, parentIndex;
	index = h->used;
	do{
		parentIndex = (index-1)/2;
		if(h->compareFunction(h->info[parentIndex], data) == 1){
			h->info[index] = h->info[parentIndex];
			h->info[parentIndex] = data;
			index = parentIndex;
		}
		else{
			break;
		}
	} while (parentIndex != 0);

	h->used++;

	return 1;

}
