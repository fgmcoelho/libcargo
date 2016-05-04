#include "vector.h"

struct vector_st {
	unsigned used;
	unsigned available;
	void** indexes;
};

static inline int vectorExpandIndexesIfNeeded(vector* v){
	if (v->available  == v->used){
		unsigned newSize = v->used*2;
		void** newIndexes = (void**)realloc(v->indexes, newSize*sizeof(void*));
		if (newIndexes == NULL){
			return 0;
		}
		v->indexes = newIndexes;
		v->available = newSize;
	}

	return 1;

}

int vectorPushBack(vector* v, void* data){

	if (vectorExpandIndexesIfNeeded(v) == 0){
		return 0;
	}

	v->indexes[v->used] = data;
	v->used++;

	return 1;
}

void vectorClear(vector** v, void (*freeFunction)(void*)){

	if (v == NULL || *v == NULL){
		return;
	}

	unsigned i;
	if (freeFunction != NULL){
		for (i = 0; i < (*v)->used; i++){
			freeFunction((*v)->indexes[i]);
		}
	}

	free((*v)->indexes);
	free(*v);
	*v = NULL;

}

int vectorAddElementAt(vector* v, void* data, unsigned at){

	if (v == NULL || at > v->used){
		return 0;
	}

	if (vectorExpandIndexesIfNeeded(v) == 0){
		return 0;
	}

	unsigned i;
	for (i = v->used; i > at; --i){
		v->indexes[i] = v->indexes[i-1];
	}

	v->indexes[at] = data;
	v->used++;

	return 1;
}

int vectorRemoveElementAt(vector* v, void (*freeFunction)(void*), unsigned at){

	if (v == NULL){
		return 0;
	}

	if (at > v->used){
		return 0;
	}

	unsigned i;
	void* dataToClear = v->indexes[at];
	for (i = at; i < v->used - 1; ++i){
		v->indexes[i] = v->indexes[i+1];
	}

	v->used--;
	if (freeFunction != NULL){
		freeFunction(dataToClear);
	}

	return 1;

}

inline void* vectorGetElementAt(vector* v, unsigned index){
	if (v == NULL || index >= v->used){
		return NULL;
	}
	return v->indexes[index];
}

inline unsigned vectorGetSize(vector* v){
	if (v != NULL){
		return v->used;
	}
	else{
		return 0;
	}
}

vector* vectorCreate(unsigned vectorSize){
	if (vectorSize == 0){
		return NULL;
	}

	vector* newVector = (vector*) malloc (sizeof(vector));
	if (newVector == NULL){
		return NULL;
	}

	newVector->indexes = (void**) malloc(sizeof(void*) * vectorSize);
	if (newVector->indexes == NULL){
		free(newVector);
		return NULL;
	}

	newVector->used = 0;
	newVector->available = vectorSize;

	return newVector;
}

int vectorSortByQuicksort(vector* v, int (*cmp)(const void*, const void*)){
    if (v == NULL || v->used == 0 || cmp == NULL){
        return 0;
    }
    if (v->used == 1){
        return 1;
    }
    return sortByQuickSort(v->indexes, v->used - 1, cmp);
}

int vectorSortByMergesort(vector* v, int (*cmp)(const void*, const void*)){
    if (v == NULL || v->used == 0 || cmp == NULL){
        return 0;
    }
    if (v->used == 1){
        return 1;
    }
    return sortByMergeSort(v->indexes, v->used, cmp);
}
