#include "heap.h"
#include <stdio.h>

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

	if(h->used == h->currentSize){
		unsigned newSize =  h->currentSize * 2;
		void** newIndexes = (void**) realloc (h->info, newSize*sizeof(heap));
		if (newIndexes == NULL){
			return 0;
		}
		h->info = newIndexes;
		h->currentSize = newSize;
	}

	h->info[h->used] = data;
	
	unsigned index, parentIndex;
	index = h->used;
	do{
		parentIndex = (index-1)/2;
		if(h->compareFunction(data, h->info[parentIndex]) > 0){
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

void* heapGetFirstElement(heap* h){

	if (h == NULL || h->used == 0){
		return NULL;
	}
	
	return h->info[0];
	
}

void* heapPopFirstElement(heap* h){

	if (h == NULL || h->used == 0){
		return NULL;
	}
	
	void* toRet = h->info[0];
	
	if (h->used == 1){
		h->info[0] = NULL;
		h->used--;
		return toRet;
	}

	h->used--;
	h->info[0] = h->info[h->used];
	h->info[h->used] = NULL;
	
	unsigned left, right, current = 0, index;
	do{
		index = current;
		left = (current * 2) + 1;
		right = (current * 2) + 2;
		
		if (left < h->used && h->compareFunction(h->info[left], h->info[current]) > 0){
			if (right < h->used && h->compareFunction(h->info[right], h->info[current]) > 0 && 
					h->compareFunction(h->info[right], h->info[left]) > 0){
				current = right;
			}
			else{
				current = left;
			}
		}
		else if (right < h->used && h->compareFunction(h->info[right], h->info[current]) > 0){
			current = right;
		}

		if (current != index){
			void* swap = h->info[current];
			h->info[current] = h->info[index];
			h->info[index] = swap;
		}

	}while(index != current);

	return toRet;

}


void heapClear(heap* h, void (*freeFunction)(void*)){
	
	if (h == NULL){
		return;
	}

	if (freeFunction != NULL){
		unsigned i;
		for (i = 0; i < h->used; ++i){
			freeFunction(h->info[i]);
		}
	}

	free(h->info);
	free(h);

}
