#include "vector.h"

static inline int vectorExpandIndexesIfNeeded(vector* v){
	if (v->available  == v->used){
		unsigned newSize = v->used*2;
		listElement** newIndexes = (listElement**)realloc(v->indexes, newSize*sizeof(listElement*));
		if (newIndexes == NULL){
			return 0;
		}
		v->indexes = newIndexes;
		v->available = newSize;
	}

	return 1;

}

int vectorPushBack(vector* v, void* data){

	if (v == NULL){
		return 0;
	}

	if (vectorExpandIndexesIfNeeded(v) == 0){
		return 0;
	}

	if(listAddElement(v->elements, data) == 0){
		return 0;
	}

	v->indexes[v->used] = v->elements->tail;
	v->used++;

	return 1;
}

void vectorClear(vector** v, void (*freeFunction)(void*)){

	if (v == NULL || *v == NULL){
		return;
	}

	listClear(&(*v)->elements, freeFunction);
	free((*v)->indexes);
	free(*v);
	*v = NULL;

}

int vectorAddElementAt(vector* v, void* data, unsigned at){

	if (v == NULL || at > v->used){
		return 0;
	}
	
	if (at == 0 && v->used == 0){
		return vectorPushBack(v, data);
	}

	listElement* le;
	if (at == 0){
		le = listAddElementAtFirstPosition(v->elements, data);
	}
	else{
		le = listAddElementAfterPosition(v->elements, v->indexes[at-1], data);
	}

	if (le == NULL){
		return 0;
	}

	if (vectorExpandIndexesIfNeeded(v) == 0){
		free(le);
		return 0;
	}

	unsigned i;
	for (i = v->used; i > at; --i){
		v->indexes[i] = v->indexes[i-1];
	}
	v->indexes[at] = le;
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

	if (at == 0){
		listRemoveElementAtFirstPosition(v->elements, freeFunction);
	}
	else{
		listRemoveElementAfterPosition(v->elements, v->indexes[at-1], freeFunction);
	}

	int i;
	for (i = at; i < v->used - 1; ++i){
		v->indexes[i] = v->indexes[i+1];
	}

	v->used--;
	
	return 1;

}

inline void* vectorGetElementAt(vector* v, unsigned index){
	if (v == NULL || index >= v->used){
		return NULL;
	}
	return v->indexes[index]->data;
}

inline unsigned vectorGetSize(vector* v){
	if (v != NULL){
		return v->used;
	}
	else{
		return 0;
	}
}

vector* vectorCreate(void){
	vector* newVector = (vector*) malloc (sizeof(vector));
	if (newVector == NULL){
		return NULL;
	}
	
	newVector->elements = listCreate();
	if (newVector->elements == NULL){
		free(newVector);
		return NULL;
	}

	newVector->indexes = (listElement**) malloc(sizeof(listElement*)*VECTOR_BASE_SIZE);
	if (newVector->indexes == NULL){
		listClear(&newVector->elements, NULL);
		free(newVector);
		return NULL;
	}
	
	newVector->used = 0;
	newVector->available = VECTOR_BASE_SIZE;

	return newVector;
}

