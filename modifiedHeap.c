#include "modifiedHeap.h"

struct modified_heap_st{
	void** info;
	int (*compareFunction)(void*, void*);
	unsigned used;
	unsigned currentSize;
	unsigned (*getIndexFunction)(void*);
	void (*setIndexFunction)(void*, unsigned);
};

modifiedHeap* modifiedHeapCreate(int (*compareFunction)(void*, void*), unsigned (*getIndexFunction)(void*),
        void (*setIndexFunction)(void*, unsigned)){

	modifiedHeap* newHeap = (modifiedHeap*)malloc(sizeof(modifiedHeap));

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
	newHeap->getIndexFunction = getIndexFunction;
	newHeap->setIndexFunction = setIndexFunction;

	return newHeap;
}

int modifiedHeapInsert(modifiedHeap* h, void* data){

	if (h == NULL){
		return 0;
	}

	if (h->used == 0){
		h->info[0] = data;
		h->setIndexFunction(data, 0);
		h->used = 1;
		return 1;
	}

	if(h->used == h->currentSize){
		unsigned newSize =  h->currentSize * 2;
		void** newIndexes = (void**) realloc (h->info, newSize*sizeof(modifiedHeap));
		if (newIndexes == NULL){
			return 0;
		}
		h->info = newIndexes;
		h->currentSize = newSize;
	}

	h->info[h->used] = data;
    h->setIndexFunction(data, h->used);

	unsigned index, parentIndex;
	index = h->used;
	do{
		parentIndex = (index-1)/2;
		if(h->compareFunction(data, h->info[parentIndex]) > 0){
			h->info[index] = h->info[parentIndex];
			h->setIndexFunction(h->info[index], index);
			h->info[parentIndex] = data;
			h->setIndexFunction(data, parentIndex);
			index = parentIndex;
		}
		else{
			break;
		}
	} while (parentIndex != 0);

	h->used++;

	return 1;

}

void* modifiedHeapGetFirstElement(modifiedHeap* h){

	if (h == NULL || h->used == 0){
		return NULL;
	}

	return h->info[0];

}

static void parentHeapify(modifiedHeap* h, unsigned start){

    if (start == 0 || start > h->used){
        return;
    }

    unsigned parent, current = start;
    do{
        parent = (current-1)/2;
        if(h->compareFunction(h->info[current], h->info[parent]) > 0){
			void* swap = h->info[parent];
			h->info[parent] = h->info[current];
			h->setIndexFunction(h->info[parent], parent);
			h->info[current] = swap;
			h->setIndexFunction(h->info[current], current);
        }
        current = parent;
    }while(current != 0);

}

static void childHeapify(modifiedHeap* h, unsigned start){

	unsigned left, right, current = start, index;
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
			h->setIndexFunction(h->info[current], current);
			h->info[index] = swap;
			h->setIndexFunction(h->info[index], index);
		}

	}while(index != current);

}

void* modifiedHeapPopFirstElement(modifiedHeap* h){

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
	h->setIndexFunction(h->info[0], 0);
	h->info[h->used] = NULL;

    childHeapify(h, 0);

	return toRet;

}

void modifiedHeapUpdatedValue(modifiedHeap* h, void* data){

    unsigned index = h->getIndexFunction(data);

    if(index == 0 && h->used == 1){
        return;
    }

    childHeapify(h, index);
    parentHeapify(h, index);

}

void modifiedHeapReset(modifiedHeap* h){
    if (h == NULL){
        return;
    }

    h->used = 0;
}

void modifiedHeapClear(modifiedHeap** h, void (*freeFunction)(void*)){

	if (h == NULL || *h == NULL){
		return;
	}

	if (freeFunction != NULL){
		unsigned i;
		for (i = 0; i < (*h)->used; ++i){
			freeFunction((*h)->info[i]);
		}
	}

	free((*h)->info);
	free((*h));

	*h = NULL;

}

void modifiedHeapIteratorStart(modifiedHeap* h, modifiedHeapIterator* it){
	if (h == NULL || it == NULL){
		return;
	}

	it->h = h;
	it->index = 0;

}

void* modifiedHeapIteratorGetFirstElement(modifiedHeapIterator* it){
	if (it == NULL || it->h == NULL || it->h->used == 0){
		return NULL;
	}

	return it->h->info[0];
}

void* modifiedHeapIteratorGetLastElement(modifiedHeapIterator* it){
	if (it == NULL || it->h == NULL || it->h->used == 0){
		return NULL;
	}

	return it->h->info[it->h->used-1];

}

void* modifiedHeapIteratorGetNextElement(modifiedHeapIterator* it){
	if (it == NULL || it->h == NULL || it->h->used <= it->index + 1){
		return NULL;
	}

	it->index++;
	return it->h->info[it->index];

}

void* modifiedHeapIteratorGetCurrentElement(modifiedHeapIterator* it){
	if (it == NULL || it->h == NULL || it->h->used <= it->index){
		return NULL;
	}

	return it->h->info[it->index];

}

void modifiedHeapIteratorReset(modifiedHeapIterator* it){
	if (it == NULL || it->h == NULL){
		return;
	}
	it->index = 0;
}

