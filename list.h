#ifndef LIST_H_GUARD__
#define LIST_H_GUARD__

#include <stdlib.h>

struct list_element_t{
	void* data;
	struct list_element_t* next;
};

typedef struct list_element_t listElement;

struct list_t{
	listElement* head, *tail;
};

typedef struct list_t list;

list* listCreate(void);
int listAddElement(list*, void*);
int listRemoveElement(list* , void* );
void listClear(list** , void (*)(void*));
void* listFindElement(list* , int(*)(void*, void*), void* );
listElement* listAddElementAtFirstPosition(list* , void* );
listElement* listAddElementAfterPosition(list* , listElement* , void* );
int listRemoveElementAfterPosition(list* , listElement* , void (*)(void*));
int listRemoveElementAtFirstPosition(list* , void(*)(void*));

struct list_iterator_t{
	list* l;
	listElement *current;
};

typedef struct list_iterator_t listIterator;

listIterator* listIteratorCreate(list* l);
void listIteratorStart(list* l, listIterator* it);
void* listIteratorGetFirstElement(listIterator*);
void* listIteratorGetNextElement(listIterator*);
void* listIteratorGetCurrentElement(listIterator*);
void listIteratorReset(listIterator*);


#endif
