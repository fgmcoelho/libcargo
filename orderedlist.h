#ifndef __ORDEREDLIST_H_GUARD
#define __ORDEREDLIST_H_GUARD

#include "vector.h"
#include "list.h"

struct ordered_list_t{
	vector* elements;
	int (*compareFunction)(void*, void*);
};

typedef struct ordered_list_t orderedList;

//TODO: definir iterators

orderedList* orderedListCreate(int (*)(void*, void*));
int orderedListInsertElement(orderedList*, void*);
int orderedListRemoveElementAt(orderedList*, void (*)(void*), unsigned);
int orderedListRemoveElement(orderedList*, void (*)(void*), void* );
void orderedListClear(orderedList**, void (*)(void*));
void* orderedListGetElementByIndex(orderedList*, unsigned);
void* orderedListFindElement(orderedList*, void*);
inline unsigned orderedListGetSize(orderedList* );
void orderedListIteratorStart(orderedList* , listIterator* );
listIterator* orderedListIteratorCreate(orderedList* );

#endif
