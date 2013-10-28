#include "vector.h"
#include "orderedlist.h"
#include "heap.h"
#include "pool.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

int testVector(){
	
	const int elementsToTest = 2000;
	const int elementsSize = 16;

	vector* v = vectorCreate();
	if (v == NULL){
		printf("%s (%d): Error starting the vector.\n", __FUNCTION__, __LINE__);
		exit(1);
	}

	pool* p = poolCreate(elementsToTest, sizeof(char) * elementsSize);

	int i;
	char* buffer;

	for (i = 0; i < elementsToTest; ++i){
		buffer = poolGetElement(p);
		if (buffer == NULL){
			printf("%s (%d): Error OOM.", __FUNCTION__, __LINE__);
			exit(1);
		}
		snprintf(buffer, elementsSize, "Teste %d.", i);
		vectorPushBack(v, buffer);
	}


	printf("%s (%d): pegando elemento 1100 [%s]\n", __FUNCTION__, __LINE__, (char*)vectorGetElementAt(v, 1100));
	assert(strcmp((char*)vectorGetElementAt(v, 1100), "Teste 1100.") == 0);
	vectorRemoveElementAt(v, NULL, 1070);
	printf("%s (%d): pegando elemente 1100 apos remocao do 1070: [%s]\n", __FUNCTION__, __LINE__, 
		(char*)vectorGetElementAt(v, 1100));
	assert(strcmp((char*)vectorGetElementAt(v, 1100), "Teste 1101.") == 0);
	
	
	buffer = poolGetElement(p);
	snprintf(buffer, elementsSize, "testando at.");
	if(vectorAddElementAt(v, buffer, 1200) == 0){
		printf("%s (%d): Error 1200.\n", __FUNCTION__, __LINE__);
		exit(1);
	}
	assert(strcmp((char*)vectorGetElementAt(v, 1200), "testando at.") == 0);
	printf("%s (%d): Testando adicao, elemento deve ser [testando at]. Valor: [%s]\n", __FUNCTION__, __LINE__, 
		(char*)vectorGetElementAt(v, 1200));

	vectorClear(&v, NULL);
	poolClear(p);

	v = vectorCreate();
	if (v == NULL){
		printf("%s (%d): Error starting the vector.\n", __FUNCTION__, __LINE__);
		exit(1);
	}

	printf("%s (%d): Criando elementos com strdup para testar double free e cleaning function.\n", __FUNCTION__, __LINE__);
	for (i = 0; i < elementsToTest; ++i){
		vectorPushBack(v, strdup("aaa"));
	}
	vectorClear(&v, free);

	printf("%s (%d): Todos os testes com vector OK, verifique leaks com o valgrind!\n", __FUNCTION__, __LINE__);

	return 1;

}

struct orderedListTest_t{
	int value;
};

typedef struct orderedListTest_t orderedListTest;

static int orderedListCompare(void* a, void* b){
	return ((orderedListTest*)a)->value - ((orderedListTest*)b)->value;

}

static int sortCmp(const void* a, const void* b){
	return *(int*)a - *(int*)b;
}

int testOrderedList(){
	
	printf("\nTesting ordered list with strings:\n");
	orderedList* ol = orderedListCreate((int(*)(void*, void*))strcmp);

	if(orderedListInsertElement(ol, strdup("aaa")) == 0){
		printf("Error %d!\n", __LINE__);
	}
	
	if(orderedListInsertElement(ol, strdup("bbb")) == 0){
		printf("Error %d!\n", __LINE__);
	}
	
	if(orderedListInsertElement(ol, strdup("fff")) == 0){
		printf("Error %d!\n", __LINE__);
	}
	
	if(orderedListInsertElement(ol, strdup("eee")) == 0){
		printf("Error %d!\n", __LINE__);
	}
	
	if(orderedListInsertElement(ol, strdup("ddd")) == 0){
		printf("Error %d!\n", __LINE__);
	}
	
	if(orderedListInsertElement(ol, strdup("bbbb")) == 0){
		printf("Error %d!\n", __LINE__);
	}
	
	if(orderedListInsertElement(ol, strdup("ccc")) == 0){
		printf("Error %d!\n", __LINE__);
	}

	if(orderedListInsertElement(ol, strdup("hhh")) == 0){
		printf("Error %d!\n", __LINE__);
	}


	if (orderedListFindElement(ol, "aaa") != NULL){
		printf("Found element that should be found!\n");
	}
	else{
		printf("Error finding elemenet.\n");
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

	orderedListIterator it;
	orderedListIteratorStart(ol, &it);
	char* element;
	for (element = orderedListIteratorGetFirstElement(&it); element != NULL; element = orderedListIteratorGetNextElement(&it)){
		printf("Element: %s.\n", element);
	}
	
	if (orderedListFindElement(ol, "bbbb") != NULL){
		printf("Found element that shouldn't be found\n");
	}
	else{
		printf("Didn't find element, as expected.\n");
	}

	orderedListClear(&ol, free);
	
	printf("\nTesting ordered with even number of integer elements:\n");
	ol = orderedListCreate(orderedListCompare);

	orderedListTest testMemory[100] = {0};
	testMemory[0].value = 1;
	testMemory[1].value = 10;
	testMemory[2].value = 20;
	testMemory[3].value = 16;
	testMemory[4].value = 12;
	testMemory[5].value = 8;

	orderedListInsertElement(ol, &testMemory[0]);
	orderedListInsertElement(ol, &testMemory[1]);
	orderedListInsertElement(ol, &testMemory[2]);
	orderedListInsertElement(ol, &testMemory[3]);
	orderedListInsertElement(ol, &testMemory[4]);
	orderedListInsertElement(ol, &testMemory[5]);

	orderedListTest findMemory;
	
	findMemory.value = 1;
	if (orderedListFindElement(ol, &findMemory) == NULL){
		printf("Element not found.\n");
	}
	
	findMemory.value = 20;
	if (orderedListFindElement(ol, &findMemory) == NULL){
		printf("Element not found.\n");
	}
	
	findMemory.value = 77;
	if (orderedListFindElement(ol, &findMemory) != NULL){
		printf("Found element that shouldn't be found.\n");
	}

	orderedListIteratorStart(ol, &it);
	orderedListTest* testElement;
	for (testElement = orderedListIteratorGetFirstElement(&it); testElement != NULL; 
			testElement = orderedListIteratorGetNextElement(&it)){
		printf("Element: %d.\n", ((orderedListTest*)testElement)->value);
	}

	orderedListClear(&ol, NULL);

	printf("\nTesting ordered with odd number of integer elements:\n");
	ol = orderedListCreate(orderedListCompare);

	testMemory[0].value = 1;
	testMemory[1].value = 10;
	testMemory[2].value = 20;
	testMemory[3].value = 16;
	testMemory[4].value = 12;
	testMemory[5].value = 8;
	testMemory[6].value = 500000;

	orderedListInsertElement(ol, &testMemory[0]);
	orderedListInsertElement(ol, &testMemory[1]);
	orderedListInsertElement(ol, &testMemory[2]);
	orderedListInsertElement(ol, &testMemory[3]);
	orderedListInsertElement(ol, &testMemory[4]);
	orderedListInsertElement(ol, &testMemory[5]);
	orderedListInsertElement(ol, &testMemory[6]);

	findMemory.value = 1;

	if (orderedListFindElement(ol, &findMemory) == NULL){
		printf("Element not found.\n");
	}
	
	findMemory.value = 500000;

	if (orderedListFindElement(ol, &findMemory) == NULL){
		printf("Element not found.\n");
	}
	
	findMemory.value = 77;
	if (orderedListFindElement(ol, &findMemory) != NULL){
		printf("Found element that shouldn't be found.\n");
	}

	orderedListIteratorStart(ol, &it);
	for (testElement = orderedListIteratorGetFirstElement(&it); testElement != NULL; 
			testElement = orderedListIteratorGetNextElement(&it)){
		printf("Element: %d.\n", ((orderedListTest*)testElement)->value);
	}

	orderedListClear(&ol, NULL);


	printf("\nTesting ordered with multiple integer with the same value:\n");
	ol = orderedListCreate(orderedListCompare);

	int  i = 0;
	testMemory[0].value = 5;
	testMemory[1].value = 8;
	testMemory[2].value = 8;
	testMemory[3].value = 8;
	testMemory[4].value = 10;
	testMemory[5].value = 1;
	testMemory[6].value = 12;
	testMemory[7].value = 12;
	testMemory[8].value = 12;
	testMemory[9].value = 3;
	testMemory[10].value = 15;
	testMemory[11].value = 15;
	testMemory[12].value = 15;
	testMemory[13].value = 20;
	testMemory[14].value = 0;

	for (i = 0; i < 15; i++){
		orderedListInsertElement(ol, &testMemory[i]);
	}
	
	orderedListIteratorStart(ol, &it);
	for (testElement = orderedListIteratorGetFirstElement(&it); testElement != NULL; 
			testElement = orderedListIteratorGetNextElement(&it)){
		printf("Element: %d.\n", ((orderedListTest*)testElement)->value);
	}

	orderedListClear(&ol, NULL);

	printf("\nTesting ordered with one hundred random values:\n");
	
	int times = 0, errorTimes = 0;
	srand(time(NULL));
	for (times = 0; times < 2000; times++){

		ol = orderedListCreate(orderedListCompare);

		int randomArray[100];
		for (i = 0; i < 100; i++){
			randomArray[i] = rand() % 50;
		}

		for (i = 0; i < 100; ++i){
			testMemory[i].value = randomArray[i];
			orderedListInsertElement(ol, &testMemory[i]);
		}

		qsort(randomArray, 100, sizeof(int), sortCmp);

		i = 0;
		int errors = 0;
		orderedListIteratorStart(ol, &it);
		for (testElement = orderedListIteratorGetFirstElement(&it); testElement != NULL; 
				testElement = orderedListIteratorGetNextElement(&it)){
			if (testElement->value != randomArray[i]){
				errors++;
			}
			//printf("%6d", testElement->value);
			i++;
		}
		//printf("\n");
		orderedListClear(&ol, NULL);
		if (errors != 0){
			printf("Test of hundred values number %d had %d errors.\n", times+1, errors);
			errorTimes++;
		}
	}
	printf("Random numbers arrays result (errors/number of arrays tested): %d/%d.\n", errorTimes, times);

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
	printf("Value on min heap should be: %d and it is %d.\n", minValue, toTest->number);
	heapClear(minHeap, free);

}


static void testPool(){
	
	const int poolElementsCount = 0x00FFFFFF;
	pool* myPool = poolCreate(poolElementsCount, sizeof(int));
	if(myPool == NULL){
		printf("Error starting pool.\n");
		exit(1);
	}

	int i = 0;
	int* x;
	
	while(poolGetElement(myPool) != NULL);
	printf("Pool is full at the limit of memory, gonna free it.\n");

	poolClear(myPool);

}

int main(){
	testVector();
	testOrderedList();
	testHeap();
	//testPool();

	return 0;
}
