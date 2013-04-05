#ifndef VECTOR_H_GUARD__
#define VECTOR_H_GUARD__

#include <stdlib.h>
#include "list.h"

#define VECTOR_BASE_SIZE 256

struct vector_t {
	unsigned used;
	unsigned available;
	listElement** indexes;
	list* elements;
};

typedef struct vector_t vector;

int vectorPushBack(vector*, void*);
void vectorClear(vector**, void (*)(void*));
int vectorRemoveElementAt(vector*, void(*)(void*), unsigned);
inline void* vectorGetElementAt(vector*, unsigned);
inline unsigned vectorGetSize(vector* );
vector* vectorCreate(void);
void vectorInialize(vector*);
int vectorAddElementAt(vector* , void* , unsigned );

#endif
