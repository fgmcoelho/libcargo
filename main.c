#include "vector.h"
#include "orderedlist.h"
#include "heap.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

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

static int compareForMinHeap(void* a, void* b){
		return ((struct heap_test_t*)a)->number < ((struct heap_test_t*)b)->number;
}

static int compareForMaxHeap(void* a, void* b){
		return ((struct heap_test_t*)a)->number > ((struct heap_test_t*)b)->number;
}

static void testHeap(void){

	srand(time(NULL));

	unsigned minValue = 3;
	unsigned maxValue = 5000000;
	struct heap_test_t* toTest;
	heap* minHeap = heapCreate(compareForMinHeap);
	
	int i;
	for (i = 0; i < 2048; ++i){
		toTest = (struct heap_test_t*)malloc(sizeof(struct heap_test_t));
		if (toTest == NULL){
			printf("Heap test failed, OOM.");
			exit (1);
		}
		toTest->number = (rand() % (maxValue - minValue - 1)) + 1;
		heapInsert(minHeap, toTest);
	}
	
	toTest = (struct heap_test_t*)malloc(sizeof(struct heap_test_t));
	if (toTest == NULL){
		printf("Heap test failed, OOM.");
		exit (1);
	}
	toTest->number = minValue;
	heapInsert(minHeap, toTest);

	toTest = heapGetFirstElement(minHeap);
	printf("Value on min heap should be: %d and it is %d.", minValue, toTest->number);
	heapClear(minHeap, free);

}

int main(){
	//testVector();
	//testOrderedList();
	testHeap();

	return 0;
}
