#ifndef __LIST_H_GUARD__
#define __LIST_H_GUARD__

#include <stdlib.h>

struct list_st;

typedef struct list_st list;

list* listCreate(void);
unsigned listGetSize(list* );
int listPushBack(list*, void*);
int listPushFront(list*, void*);
int listRemoveElement(list* , void(*)(void*), void* );
int listRemoveElementIf(list*, void(*)(void*), int (*)(void*));
void* listPopFirstElement(list*);
void* listPopLastElement(list*);
void* listGetFirstElement(list*);
void* listGetLastElement(list*);
void* listFindElement(list* , int(*)(void*, void*), void* );
int listMergeLists(list** , list** );
void listClear(list** , void (*)(void*));

struct list_iterator_st {
	list* l;
	void *current;
};

typedef struct list_iterator_st listIterator;

listIterator* listIteratorCreate(list* l);
void listIteratorStart(list* l, listIterator* it);
void* listIteratorGetFirstElement(listIterator*);
void* listIteratorGetLastElement(listIterator*);
void* listIteratorGetNextElement(listIterator*);
void* listIteratorGetCurrentElement(listIterator*);
void listIteratorReset(listIterator*);

int listIteratorRemoveCurrent(listIterator*, void(*)(void*));
int listIteratorAddElementAfter(listIterator*, void*);
int listIteratorAddElementBefore(listIterator*, void*);


#endif
