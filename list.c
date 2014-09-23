#include "list.h"

struct list_element_st{
	void* data;
	struct list_element_st* next, *former;
};

typedef struct list_element_st listElement;

struct list_st{
	unsigned size;
	listElement* head, *tail;
};

list* listCreate(){

	list* newList = (list*) malloc (sizeof(list));
	if (newList != NULL){
		newList->size = 0;
		newList->head = NULL;
		newList->tail = NULL;
	}

	

	return newList;
}

unsigned listGetSize(list* l){

	if (l == NULL){
		return 0;
	}
	else{
		return l->size;
	}

}

int listPushBack(list* l, void* data){

	if (l == NULL){
		return 0;
	}

	listElement* newListElement = (listElement*) malloc (sizeof(listElement));

	if (newListElement == NULL){
		return 0;
	}

	newListElement->data = data;

	if (l->head == NULL){
		l->head = newListElement;
		l->tail = newListElement;
		newListElement->next = NULL;
		newListElement->former = NULL;
	}
	else{
		l->tail->next = newListElement;
		newListElement->next = NULL;
		newListElement->former = l->tail;
		l->tail = newListElement;
	}

	l->size++;

	return 1;
}

int listPushFront(list* l, void* data){

	if (l == NULL){
		return 0;
	}

	listElement* newListElement = (listElement*) malloc (sizeof(listElement));

	if (newListElement == NULL){
		return 0;
	}

	newListElement->data = data;

	if (l->head == NULL){
		l->head = newListElement;
		l->tail = newListElement;
		newListElement->next = NULL;
		newListElement->former = NULL;
	}
	else{
		l->head->former = newListElement;
		newListElement->former = NULL;
		newListElement->next = l->head;
		l->head = newListElement;
	}

	l->size++;

	return 1;

}

int listRemoveElement(list *l, void (*freeFunction)(void*), void* toRemove){

	if (l == NULL || l->head == NULL){
		return 0;
	}
	else{
		listElement* former = l->head;
		if (former->data == toRemove){
			if (former->next == NULL){
				l->head = NULL;
				l->tail = NULL;
				l->size--;
				if (freeFunction != NULL){
					freeFunction(former->data);
				}
				free(former);
				return 1;
			}
			l->head = former->next;
			former->next->former = NULL;
			if (l->head == NULL){
				l->tail = NULL;
			}
			l->size--;
			if (freeFunction != NULL){
				freeFunction(former->data);
			}
			free(former);
			return 1;
		}

		listElement* aux = former->next;
		while(aux != NULL){
			if (aux->data == toRemove){
				if (l->tail == aux){
					l->tail = former;
					former->next = NULL;
				}
				else{
					former->next = aux->next;
					aux->next->former = former;
				}
				if (freeFunction != NULL){
					freeFunction(aux->data);
				}
				free(aux);
				l->size--;
				return 1;
			}
			former = aux;
			aux = aux->next;
		}
		return 0;
	}

}

int listRemoveElementIf(list* l, void(*freeFunction)(void*), int (*compareFunction)(void*)){

	if (l == NULL || l->head == NULL){
		return 0;
	}

	listElement *toFree, *aux = l->head;
	while(aux != NULL){
		if (compareFunction(aux->data) == 1){
			toFree = aux;

			if (aux->next != NULL){
				aux->next->former = aux->former;
			}
			if (aux->former != NULL){
				aux->former->next = aux->next;
			}

			if (aux == l->head){
				l->head = aux->next;
			}
			if (aux == l->tail){
				l->tail = aux->former;
			}

			aux = aux->next;

			if (freeFunction != NULL){
				freeFunction(toFree->data);
			}
			free(toFree);
			l->size--;

		}
		else{
			aux = aux->next;
		}
	}

	return 1;

}

void* listPopFirstElement(list* l){

	if (l == NULL){
		return NULL;
	}

	if (l->head != NULL){
		void* data = l->head->data;
		listElement* toFree = l->head;
		if (l->head == l->tail){
			l->head = NULL;
			l->tail = NULL;
		}
		else{
			l->head = l->head->next;
			l->head->former = NULL;
		}
		free(toFree);
		l->size--;
		return data;
	}
	else{
		return NULL;
	}

}

void* listGetFirstElement(list* l){
	if (l == NULL){
		return NULL;
	}
	return l->head->data;
}

void* listGetLastElement(list* l){
	if (l == NULL){
		return NULL;
	}
	return l->tail->data;
}


void* listPopLastElement(list* l){

	if (l == NULL){
		return NULL;
	}

	if (l->tail != NULL){
		void* data = l->tail->data;
		listElement* toFree = l->tail;
		if (l->head == l->tail){
			l->head = NULL;
			l->tail = NULL;
		}
		else{
			l->tail = l->tail->former;
			l->tail->next = NULL;
		}
		free(toFree);
		l->size--;
		return data;
	}
	else{
		return NULL;
	}

}

void* listFindElement(list* l, int(*compareFunction)(void*, void*), void* toFind){

	if (l == NULL || compareFunction == NULL || toFind == NULL){
		return NULL;
	}

	listElement* aux = l->head;

	while(aux != NULL){
		if (compareFunction(aux->data, toFind) == 1){
			return aux->data;
		}
		aux = aux->next;
	}

	return NULL;
}

int listMergeLists(list** dstList, list** srcList){
	if (*dstList == NULL || *srcList == NULL){
		return 0;
	}

	if ((*dstList)->head == NULL){
		(*dstList)->head = (*srcList)->head;
		(*dstList)->tail = (*srcList)->tail;
		(*dstList)->size = (*srcList)->size;
		free(*srcList);
		*srcList = NULL;
		return 1;
	}
	else{
		(*srcList)->head->former = (*dstList)->tail;
		(*dstList)->tail->next = (*srcList)->head;
		(*dstList)->tail = (*srcList)->tail;
		(*dstList)->size += (*srcList)->size;
		free(*srcList);
		*srcList = NULL;
		return 1;
	}

}

void listClear(list** l, void (*func)(void*)){

	if (l == NULL || *l == NULL){
		return;
	}

	listElement* aux = (*l)->head, *aux2;
	while(aux != NULL){
		aux2 = aux->next;
		if (func != NULL){
			func(aux->data);
		}
		free(aux);
		aux = aux2;
	}
	free(*l);
	*l = NULL;
}

void listIteratorStart(list* l, listIterator* it){

	it->l = l;
	it->current = l->head;

}

listIterator* listIteratorCreate(list* l){

	listIterator* it = (listIterator*) malloc(sizeof(listIterator));
	if (it == NULL){
		return NULL;
	}

	it->l = l;
	it->current = l->head;

	return it;

}

void* listIteratorGetFirstElement(listIterator* it){

	if (it == NULL){
		return NULL;
	}
	else {
		if (it->l->head == NULL){
			return NULL;
		}
		return it->l->head->data;
	}

}

void* listIteratorGetLastElement(listIterator* it){
	if (it == NULL){
		return NULL;
	}
	else{
		return it->l->tail->data;
	}
}

void* listIteratorGetNextElement(listIterator* it){

	if (it == NULL || it->current == NULL || ((listElement*)it->current)->next == NULL){
		return NULL;
	}
	else{
		it->current = ((listElement*)it->current)->next;
		return ((listElement*)it->current)->data;
	}

}

void* listIteratorGetCurrentElement(listIterator* it){

	if(it == NULL){
		return NULL;
	}
	else{
		return ((listElement*)it->current)->data;
	}

}

void listIteratorReset(listIterator* it){

	if (it == NULL){
		return;
	}

	it->current = it->l->head;

}

int listIteratorRemoveCurrent(listIterator* it, void(*freeFunction)(void*)){

	if(it == NULL || it->l == NULL || it->l->head == NULL){
		return 0;
	}

	list* l = it->l;
	listElement* toFree;
	if (it->current == l->head){
		if (freeFunction != NULL){
			freeFunction(listPopFirstElement(l));
		}
		else{
			listPopFirstElement(l);
		}
		it->current = l->head;

		return 1;
	}

	if (it->current == l->tail){
		if (freeFunction != NULL){
			freeFunction(listPopLastElement(l));
		}
		else{
			listPopLastElement(l);
		}
		it->current = NULL;
		return 1;
	}

	l->size--;
	((listElement*)it->current)->former->next = ((listElement*)it->current)->next;
	((listElement*)it->current)->next->former = ((listElement*)it->current)->former;
	if (freeFunction != NULL){
		freeFunction(((listElement*)it->current)->data);
	}
	toFree = it->current;
	it->current = ((listElement*)it->current)->next;
	free(toFree);

	return 1;

}

int listIteratorAddElementAfter(listIterator* it, void* data){

	if(it == NULL || it->l == NULL || it->l->head == NULL){
		return 0;
	}

	list* l = it->l;
	if (it->current == l->tail){
		return listPushBack(l, data);
	}

	listElement* newElement = (listElement*) malloc (sizeof(newElement));
	if (newElement == NULL){
		return 0;
	}

	l->size++;
	newElement->next = ((listElement*)it->current)->next;
	((listElement*)it->current)->next->former = newElement;
	newElement->former = it->current;
	((listElement*)it->current)->next = newElement;

	return 1;

}

int listIteratorAddElementBefore(listIterator* it, void* data){
	if(it == NULL || it->l == NULL || it->l->head == NULL){
		return 0;
	}

	list* l = it->l;
	if (it->current == l->head){
		return listPushFront(l, data);
	}

	listElement* newElement = (listElement*) malloc (sizeof(newElement));
	if (newElement == NULL){
		return 0;
	}

	l->size++;
	((listElement*)it->current)->former->next = newElement;
	newElement->former = ((listElement*)it->current)->former;
	newElement->next = it->current;
	((listElement*)it->current)->former = newElement;

	return 1;

}


