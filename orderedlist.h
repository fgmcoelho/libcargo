#ifndef __ORDEREDLIST_H_GUARD__
#define __ORDEREDLIST_H_GUARD__

struct ordered_list_st;

typedef struct ordered_list_st orderedList;

orderedList* orderedListCreate(unsigned size, int (*compareFunction)(void*, void*));
int orderedListInsertElement(orderedList*, void*);
int orderedListRemoveElementAt(orderedList*, void (*)(void*), unsigned);
int orderedListRemoveElement(orderedList*, void (*)(void*), void* );
void orderedListClear(orderedList**, void (*)(void*));
void* orderedListGetElementAt(orderedList*, unsigned);
void* orderedListFindElement(orderedList*, void*);
inline unsigned orderedListGetSize(orderedList* );

struct ordered_list_iterator_st{
	orderedList* ol;
	unsigned index;
};

typedef struct ordered_list_iterator_st orderedListIterator;

orderedListIterator* orderedListIteratorCreate(orderedList* );
void orderedListIteratorStart(orderedList* , orderedListIterator* );
void* orderedListIteratorGetFirstElement(orderedListIterator*);
void* orderedListIteratorGetNextElement(orderedListIterator*);
void* orderedListIteratorGetCurrentElement(orderedListIterator*);
void orderedListIteratorReset(orderedListIterator*);

#endif
