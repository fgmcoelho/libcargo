#ifndef __VECTOR_H_GUARD__
#define __VECTOR_H_GUARD__

#include <stdlib.h>

struct vector_st;

typedef struct vector_st vector;

int vectorPushBack(vector*, void*);
void vectorClear(vector**, void (*)(void*));
int vectorRemoveElementAt(vector*, void(*)(void*), unsigned);
inline void* vectorGetElementAt(vector*, unsigned);
inline unsigned vectorGetSize(vector* );
vector* vectorCreate(unsigned vectorSize);
int vectorAddElementAt(vector* , void* , unsigned );

#endif
