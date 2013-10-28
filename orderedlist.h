#ifndef __ORDEREDLIST_H_GUARD
#define __ORDEREDLIST_H_GUARD

#include "vector.h"

#include <stdio.h>

struct ordered_list_t{
	vector* elements;
	int (*compareFunction)(void*, void*);
};

typedef struct ordered_list_t orderedList;

orderedList* orderedListCreate(int (*)(void*, void*));
int orderedListInsertElement(orderedList*, void*);
int orderedListRemoveElementAt(orderedList*, void (*)(void*), unsigned);
int orderedListRemoveElement(orderedList*, void (*)(void*), void* );
void orderedListClear(orderedList**, void (*)(void*));
void* orderedListGetElementByIndex(orderedList*, unsigned);
void* orderedListFindElement(orderedList*, void*);
inline unsigned orderedListGetSize(orderedList* );

struct ordered_list_iterator_t{
	orderedList* ol;
	unsigned index;
};

typedef struct ordered_list_iterator_t orderedListIterator;

orderedListIterator* orderedListIteratorCreate(orderedList* );
void orderedListIteratorStart(orderedList* , orderedListIterator* );
void* orderedListIteratorGetFirstElement(orderedListIterator*);
void* orderedListIteratorGetNextElement(orderedListIterator*);
void* orderedListIteratorGetCurrentElement(orderedListIterator*);
void orderedListIteratorReset(orderedListIterator*);

#endif
