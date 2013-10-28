	#include "orderedlist.h"
//#include <stdio.h>

static unsigned orderedListFindElementIndex(orderedList* , void* , int* );

orderedList* orderedListCreate(int (*compareFunction)(void*, void*)){

	orderedList* newOrderedList = (orderedList*) malloc(sizeof(orderedList));
	if (newOrderedList != NULL){
		newOrderedList->elements = vectorCreate();
		if (newOrderedList->elements != NULL){
			newOrderedList->compareFunction = compareFunction;
		}
		else{
			free(newOrderedList);
			newOrderedList = NULL;
		}
	}

	return newOrderedList;
}

int orderedListInsertElement(orderedList* ol, void* data){

	if (ol == NULL){
		return 0;
	}

	//printf ("Trying to insert: %d.\n", *(int*)data);
	unsigned size = vectorGetSize(ol->elements);
	if (size == 0){
		return vectorPushBack(ol->elements, data);
	}

	int min = 0, max = size - 1, index, formerIndex = size;
	int result;
	index = ((max + min)/2) + (max + min)%2;
	do{
		result = ol->compareFunction(data, vectorGetElementAt(ol->elements, index));
		//printf("Compared %d with %d [index: %d], result -> %d | min %d | max %d .\n", 
		//	*(int*)data, *(int*)(vectorGetElementAt(ol->elements, index)), index, result, min, max);
		
		if (result < 0){
			formerIndex = index;
			max = index;
			index = ((max + min)/2) + (max + min)%2;
		}
		else if (result > 0){
			formerIndex = index;
			min = index;
			index = (max + min)/2 + (max + min)%2;
		}

	}while (index != formerIndex && (result != 0) && (min != max));

	if (index == formerIndex && result < 0 && index != 0 && 	
			ol->compareFunction(data, vectorGetElementAt(ol->elements, index -1)) < 0){
		index--;
	}
	else{
		if (min == max && result > 0){
			index = size;
		}
		if (min == max && result < 0){
			index = min;
		}
	}
	//printf("Inserting element at %d.\n", index);

	return vectorAddElementAt(ol->elements, data, index);

}

int orderedListRemoveElementAt(orderedList* ol, void (*freeFunction)(void*), unsigned at){
	if (ol == NULL){
		return 0;
	}

	return vectorRemoveElementAt(ol->elements, freeFunction, at);
}

int orderedListRemoveElement(orderedList* ol, void (*freeFunction)(void*), void* data){
	int result;
	unsigned index = orderedListFindElementIndex(ol, data, &result);
	if (result == 0){
		return 0;
	}
	return vectorRemoveElementAt(ol->elements, freeFunction, index);
}

void orderedListClear(orderedList** ol, void (*freeFunction)(void*)){

	if (ol == NULL || *ol == NULL){
		return;
	}

	vectorClear(&(*ol)->elements, freeFunction);
	free(*ol);
	*ol = NULL;

}

void* orderedListGetElementAt(orderedList* ol, unsigned at){
	if (ol == NULL){
		return NULL;
	}
	return vectorGetElementAt(ol->elements, at);
}

static unsigned orderedListFindElementIndex(orderedList* ol, void* data, int* searchResult){
	
	//printf ("Trying to find: %s.\n", (char*)data);
	
	unsigned size = vectorGetSize(ol->elements);
	if (size == 0){
		if (ol->compareFunction(data, vectorGetElementAt(ol->elements, 0)) == 0){
			*searchResult = 1;
		}
		else{
			*searchResult = 0;
		}
		return 0;
	}
	
	int min = 0, max = size - 1, index;
	int result;
	index = (max + min)/2;
	do{
		result = ol->compareFunction(data, vectorGetElementAt(ol->elements, index));
		//printf("Compared %s with %s [index: %d], result -> %d | min %d | max %d .\n", 
		//	data, vectorGetElementAt(ol->elements, index), index, result, min, max);

		if (result < 0){
			max = index -1;
			index = ((max + min)/2);
		}
		else if (result > 0){
			min = index + 1;
			index = ((max + min)/2);
		}
		else{
			*searchResult = 1;
			return index;
		}

	}while (max >= min);

	*searchResult = 0;
	return 0;

}

void* orderedListFindElement(orderedList* ol, void* data){
	if (ol == NULL){
		return NULL;
	}

	unsigned size = vectorGetSize(ol->elements);
	if (size == 0){
		return NULL;
	}

	int searchResult;
	unsigned index = orderedListFindElementIndex(ol, data, &searchResult);
	if (searchResult == 0){	
		return NULL;
	}
	else{
		return vectorGetElementAt(ol->elements, index);
	}
}

inline unsigned orderedListGetSize(orderedList* ol){
	if(ol == NULL){
		return 0;
	}
	return vectorGetSize(ol->elements);
}


orderedListIterator* orderedListIteratorCreate(orderedList* ol){
	
	if (ol == NULL){
		return NULL;
	}
	
	orderedListIterator* it = (orderedListIterator*)malloc(sizeof(orderedListIterator));
	if (it == NULL){
		return NULL;
	}

	it->ol = ol;
	it->index = 0;

}

void orderedListIteratorStart(orderedList* ol, orderedListIterator* it){
	it->ol = ol;
	it->index = 0;
}

void* orderedListIteratorGetFirstElement(orderedListIterator* it){

	if (it == NULL){
		return NULL;
	}
	return orderedListGetElementAt(it->ol, 0);
	
}

void* orderedListIteratorGetNextElement(orderedListIterator* it){
	
	if (it == NULL){
		return NULL;
	}
	
	it->index++;
	return orderedListGetElementAt(it->ol, it->index);

}

void* orderedListIteratorGetCurrentElement(orderedListIterator* it){
	if (it == NULL){
		return NULL;
	}
	
	return orderedListGetElementAt(it->ol, it->index);

}

void orderedListIteratorReset(orderedListIterator* it){
	it->index = 0;
}


