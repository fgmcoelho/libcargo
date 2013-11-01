#ifndef LIST_H_GUARD__
#define LIST_H_GUARD__

#include <stdlib.h>

struct list_element_t{
	void* data;
	struct list_element_t* next, *former;
};

typedef struct list_element_t listElement;

struct list_t{
	unsigned size;
	listElement* head, *tail;
};

typedef struct list_t list;

list* listCreate(void);
unsigned listGetSize(list* );
int listPushBack(list*, void*);
int listPushFront(list*, void*);
int listRemoveElement(list* , void(*)(void*), void* );
int listRemoveElementIf(list*, void(*)(void*), int (*)(void*));
void* listPopFirstElement(list*);
void* listPopLastElement(list*);
void* listFindElement(list* , int(*)(void*, void*), void* );
int listMergeLists(list** , list** );
void listClear(list** , void (*)(void*));

struct list_iterator_t{
	list* l;
	listElement *current;
};

typedef struct list_iterator_t listIterator;

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
