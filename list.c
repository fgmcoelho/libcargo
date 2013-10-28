#include "list.h"

list* listCreate(){
	
	list* newList = (list*) malloc (sizeof(list));
	if (newList != NULL){
		newList->head = NULL;
		newList->tail = NULL;
	}

	return newList;
}

int listAddElement(list* l, void* data){

	if (l == NULL){
		return 0;
	}
	
	listElement* newListElement = (listElement*) malloc (sizeof(list));

	if (newListElement == NULL){
		return 0;
	}

	newListElement->next = NULL;
	newListElement->data = data;

	if (l->head == NULL){
		l->head = newListElement;
		l->tail = newListElement;
	}
	else{
		l->tail->next = newListElement;
		l->tail = newListElement;
	}

	return 1;
}

listElement* listAddElementAtFirstPosition(list* l, void* data){
	
	if (l == NULL){
		return 0;
	}
	
	listElement* newListElement = (listElement*) malloc (sizeof(list));
	if (newListElement != NULL){
		newListElement->data = data;
		newListElement->next = l->head;
		l->head = newListElement;
	}

	return newListElement;

}

listElement* listAddElementAfterPosition(list* l, listElement* position, void* data){
	if (l == NULL || position == NULL){
		return 0;
	}
	
	listElement* newListElement = (listElement*) malloc (sizeof(list));

	if (newListElement != NULL){
		newListElement->data = data;

		if (l->tail == position){
			l->tail->next = newListElement;
			l->tail = newListElement;
			newListElement->next = NULL;
		}
		else{
			newListElement->next = position->next;
			position->next = newListElement;
		}
	}
	
	return newListElement;
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

void* listPopFirstElement(list* l){

	if (l == NULL){
		return NULL;
	}

	if (l->head != NULL){
		void* data = l->head->data;
		listElement* toFree = l->head;
		l->head = l->head->next;
		free(toFree);
		return data;
	}
	else{
		return NULL;
	}

}

int listRemoveElement(list *l, void* toRemove){

	if (l == NULL || l->head == NULL){
		return 0;
	}
	else{
		listElement* former = l->head;
		if (former->data == toRemove){
			l->head = former->next;
			if (l->head == NULL){
				l->tail = NULL;
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
				}
				free(aux);
				return 1;
			}
			former = aux;
			aux = aux->next;
		}
		return 0;
	}

}

int listMergeLists(list** dstList, list** srcList){
	if (*dstList == NULL || *srcList == NULL){
		return 0;
	}

	if ((*dstList)->head == NULL){
		(*dstList)->head = (*srcList)->head;
		(*dstList)->tail = (*srcList)->tail;
		free(*srcList);
		*srcList = NULL;
		return 1;
	}
	else{
		(*dstList)->tail->next = (*srcList)->head;
		free(*srcList);
		*srcList = NULL;
		return 1;
	}

}


int listRemoveElementAfterPosition(list* l, listElement* position, void (*freeFunction)(void*)){

	if (l == NULL || position == NULL || l->tail == position){
		return 0;
	}

	if (position->next == l->tail){
		l->tail = position;
		if(freeFunction != NULL){
			freeFunction(position->next->data);
		}
		free(position->next);
		position->next = NULL;
	}
	else{
		listElement* toRemove = position->next;
		position->next = toRemove->next;
		if (freeFunction != NULL){
			freeFunction(toRemove->data);
		}
		free(toRemove);
	}

	return 1;

}

int listRemoveElementAtFirstPosition(list* l, void(*freeFunction)(void*)){
	if (l == NULL){
		return 0;
	}
	if (l->head == NULL){
		return 1;
	}

	listElement* newHead = l->head->next;
	if (freeFunction != NULL){
		freeFunction(l->head->data);
	}
	free(l->head);
	l->head = newHead;
	
	return 1;
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

void* listIteratorGetNextElement(listIterator* it){

	if (it == NULL || it->current == NULL || it->current->next == NULL){
		return NULL;
	}
	else{
		it->current = it->current->next;
		return it->current->data;
	}

}

void* listIteratorGetCurrentElement(listIterator* it){

	if(it == NULL){
		return NULL;
	}
	else{
		return it->current->data;
	}

}

void listIteratorReset(listIterator* it){

	if (it == NULL){
		return;
	}

	it->current = it->l->head;

}


