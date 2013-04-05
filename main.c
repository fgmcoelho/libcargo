#include "vector.h"
#include "orderedlist.h"
#include "heap.h"
#include <stdio.h>
#include <string.h>


int testVector(){
	
	vector* v = vectorCreate();
	if (v == NULL){
		printf("Error starting the vector.\n");
		return 0;
	}

	int i = 0;

	char buffer[256];
	for (i = 0; i < 2000; ++i){
		snprintf(buffer, sizeof(buffer), "Teste %d.", i);
		vectorPushBack(v, strdup(buffer));
	}


	printf("%s\n", (char*)vectorGetElementAt(v, 1100));
	vectorRemoveElementAt(v, free, 1070);
	printf("%s\n", (char*)vectorGetElementAt(v, 1100));
	
	if(vectorAddElementAt(v, strdup("testando at."), 1200) == 0){
		printf("Error 1200.\n");
	}
	printf("%s\n", (char*)vectorGetElementAt(v, 1200));
	
	if(vectorAddElementAt(v, strdup("testando at."), 0) == 0){
		printf("Error 0.\n");
	}
	printf("%s\n", (char*)vectorGetElementAt(v, 1200));

	vectorClear(&v, free);
	
	return 1;

}

int testOrderedList(){
	
	orderedList* ol = orderedListCreate((int(*)(void*, void*))strcmp);

	if(orderedListInsertElement(ol, strdup("aaaa")) == 0){
		printf("Error %d!\n", __LINE__);
	}
	
	if(orderedListInsertElement(ol, strdup("bbbb")) == 0){
		printf("Error %d!\n", __LINE__);
	}
	
	if(orderedListInsertElement(ol, strdup("cccc")) == 0){
		printf("Error %d!\n", __LINE__);
	}
	
	if(orderedListInsertElement(ol, strdup("aaaa")) == 0){
		printf("Error %d!\n", __LINE__);
	}
	
	if(orderedListInsertElement(ol, strdup("bbbb")) == 0){
		printf("Error %d!\n", __LINE__);
	}
	
	if(orderedListInsertElement(ol, strdup("b")) == 0){
		printf("Error %d!\n", __LINE__);
	}
	
	if(orderedListInsertElement(ol, strdup("c")) == 0){
		printf("Error %d!\n", __LINE__);
	}

	if(orderedListInsertElement(ol, strdup("ccccc")) == 0){
		printf("Error %d!\n", __LINE__);
	}


	if (orderedListFindElement(ol, "aaaa") != NULL){
		printf("Found element that should be found!\n");
	}
	else{
		printf("Error finding elemenet.\n");
	}
	
	if (orderedListFindElement(ol, "dddd") != NULL){
		printf("Found element that shouldn't be found\n");
	}
	else{
		printf("Didn't find element, as expected.\n");
	}

	if (orderedListRemoveElement(ol, free, "bbbb") == 0){
		printf("Error %d!\n", __LINE__);
	}
	
	if (orderedListRemoveElement(ol, free, "fjkdmlgf") != 0){
		printf("Error %d!\n", __LINE__);
	}

	if (orderedListRemoveElementAt(ol, free, 200) != 0){
		printf("Error %d!\n", __LINE__);
	}
	
	if (orderedListRemoveElementAt(ol, free, 2) == 0){
		printf("Error %d!\n", __LINE__);
	}

	if(orderedListInsertElement(ol, strdup("dd")) == 0){
		printf("Error %d!\n", __LINE__);
	}

	listIterator it;
	orderedListIteratorStart(ol, &it);
	char* element;
	for (element = listIteratorGetFirstElement(&it); element != NULL; element = listIteratorGetNextElement(&it)){
		printf("Element: %s.\n", element);
	}

	orderedListClear(&ol, free);

	return 0;

}

struct heap_test_t {
	int number;
};

static int compareForHeap(void* a, void* b){
	return ((struct heap_test_t*)a)->number > ((struct heap_test_t*)b)->number;
}

static void testHeap(void){

	heap* myHeap = heapCreate(compareForHeap);
	
	struct heap_test_t heapTestEntries[10];
	
	heapTestEntries[0].number = 11;
	heapTestEntries[1].number = 5;
	heapTestEntries[2].number = 8;
	heapTestEntries[3].number = 3;
	heapTestEntries[4].number = 4;
	heapTestEntries[5].number = 15;
	
	heapInsert(myHeap, &heapTestEntries[0]);
	heapInsert(myHeap, &heapTestEntries[1]);
	heapInsert(myHeap, &heapTestEntries[2]);
	heapInsert(myHeap, &heapTestEntries[3]);
	heapInsert(myHeap, &heapTestEntries[4]);
	heapInsert(myHeap, &heapTestEntries[5]);

	int i;
	for (i = 0; i < 6; ++i){
		printf("%d.\n", ((struct heap_test_t*)myHeap->info[i])->number);
	}

}

int main(){
	//testVector();
	//testOrderedList();
	testHeap();

	return 0;
}
