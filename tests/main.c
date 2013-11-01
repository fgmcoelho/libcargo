#include "list.h"
#include "vector.h"
#include "orderedlist.h"
#include "heap.h"
#include "pool.h"
#include "refcount.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

static const int LIST_NUMBER_OF_ELEMENTS = 10;
static const int LIST_MAX_ELEMENT_VALUE = 10000;
static const int LIST_TOTAL_RANDOM_TESTS = 100000;

static int isOdd(void* a){
	return *((int*)a)%2;
}

static int isEven(void* a){
	return !(*((int*)a)%2);
}

static int* allocateNewEvenNumber(void){
	int *newElement = (int*) malloc (sizeof(int));
	if (newElement == NULL){
		printf("%s (%d): Error OOM.\n", __FUNCTION__, __LINE__);
		exit(1);
	}
	*newElement = rand() % LIST_MAX_ELEMENT_VALUE;
	if (*newElement % 2 != 0){
		(*newElement)++;
	}

	return newElement;
}

static int* allocateNewOddNumber(void){
	int* newElement = allocateNewEvenNumber();
	(*newElement)++;
	return newElement;
}

static void testList(){


	srand(time(NULL));
	list* firstList = listCreate();
	list* secondList = listCreate();

	int i;
	for (i = 0; i < LIST_NUMBER_OF_ELEMENTS; i++){
		
		if (listPushBack(firstList, allocateNewEvenNumber()) == 0){
			printf("%s (%d): Error adding element to list.\n", __FUNCTION__, __LINE__);
			exit(1);
		}

	}
	
	for (i = 0; i < LIST_NUMBER_OF_ELEMENTS; i++){
		if (listPushBack(secondList, allocateNewOddNumber()) == 0){
			printf("%s (%d): Error adding element to list.\n", __FUNCTION__, __LINE__);
			exit(1);
		}
	}
	
	listIterator it;
	int* currentElement;
	
	printf("Printing first list:\n");
	listIteratorStart(firstList, &it);
	for (currentElement = listIteratorGetFirstElement(&it); currentElement != NULL; 
			currentElement = listIteratorGetNextElement(&it)){
		
		printf("%d.\n", *currentElement);
	
	}

	if (listRemoveElement(firstList, free, listIteratorGetFirstElement(&it)) == 0){
		printf("%s (%d): Error removing first element.\n", __FUNCTION__, __LINE__);
		exit(1);
	}
	
	if(listRemoveElement (firstList, free, listIteratorGetLastElement(&it)) == 0){
		printf("%s (%d): Error removing last element.\n", __FUNCTION__, __LINE__);
		exit(1);
	}

	assert(listGetSize(firstList) == LIST_NUMBER_OF_ELEMENTS - 2);

	printf("Printing second list:\n");
	listIteratorStart(secondList, &it);
	for (currentElement = listIteratorGetFirstElement(&it); currentElement != NULL; 
			currentElement = listIteratorGetNextElement(&it)){
		
		printf("%d.\n", *currentElement);
	
	}

	if (listMergeLists(&firstList, &secondList) == 0){
		printf("%s (%d): Error merging lists.\n", __FUNCTION__, __LINE__);
		exit(1);
	}

	assert(secondList == NULL);
	assert(listGetSize(firstList) == (LIST_NUMBER_OF_ELEMENTS * 2) - 2);

	listIteratorStart(firstList, &it);
	printf("Printing merged lists:\n");
	for (currentElement = listIteratorGetFirstElement(&it); currentElement != NULL; 
			currentElement = listIteratorGetNextElement(&it)){
		
		printf("%d.\n", *currentElement);
	
	}
	
	assert(listRemoveElementIf(firstList, free, isOdd) == 1);
	assert(listGetSize(firstList) == LIST_NUMBER_OF_ELEMENTS -2);
	
	listIteratorStart(firstList, &it);
	printf("Printing merged lists without the odd numbers:\n");
	for (currentElement = listIteratorGetFirstElement(&it); currentElement != NULL; 
			currentElement = listIteratorGetNextElement(&it)){
		
		printf("%d.\n", *currentElement);
	
	}

	if (listPushFront(firstList, allocateNewOddNumber()) == 0){
		printf("%s (%d): Error adding element to list.\n", __FUNCTION__, __LINE__);
		exit(1);
	}

	if (listPushFront(firstList, allocateNewEvenNumber()) == 0){
		printf("%s (%d): Error adding element to list.\n", __FUNCTION__, __LINE__);
		exit(1);
	}

	assert(listGetSize(firstList) == LIST_NUMBER_OF_ELEMENTS);

	listIteratorStart(firstList, &it);
	printf("Printing merged lists with a single odd number at second position:\n");
	for (currentElement = listIteratorGetFirstElement(&it); currentElement != NULL; 
			currentElement = listIteratorGetNextElement(&it)){
		
		printf("%d.\n", *currentElement);
	
	}

	listRemoveElementIf(firstList, free, isEven);
	assert(listGetSize(firstList) == 1);

	listIteratorStart(firstList, &it);
	printf("Printing merged lists with a single odd number:\n");
	for (currentElement = listIteratorGetFirstElement(&it); currentElement != NULL; 
			currentElement = listIteratorGetNextElement(&it)){
		
		printf("%d.\n", *currentElement);
	
	}
	
	listIteratorStart(firstList, &it);
	listIteratorAddElementBefore(&it, allocateNewEvenNumber());
	assert(listGetSize(firstList) == 2);
	listIteratorAddElementAfter(&it, allocateNewOddNumber());
	assert(listGetSize(firstList) == 3);

	listIteratorReset(&it);
	printf("Printing merged lists with three elements:\n");
	for (currentElement = listIteratorGetFirstElement(&it); currentElement != NULL; 
			currentElement = listIteratorGetNextElement(&it)){
		
		printf("%d.\n", *currentElement);
	
	}

	listClear(&firstList, free);
	assert(firstList == NULL);

	firstList = listCreate();
	int odd = 0, even = 0;
	for (i = 0; i < LIST_TOTAL_RANDOM_TESTS; ++i){
		int roll = rand() % 10;
		void* data;
		if (roll == 1){
			printf("PUSH BACK\n");
			assert(listPushBack(firstList, allocateNewEvenNumber()));
			even++;
		}
		else if (roll == 2){
			printf("PUSH BACK\n");
			assert(listPushBack(firstList, allocateNewOddNumber()));
			odd++;
		}
		else if (roll == 3){
			printf("POP FIRST\n");
			if (listGetSize(firstList) == 0) {
				data = listPopFirstElement(firstList);
				assert(data == NULL);
			}
			else{
				data = listPopFirstElement(firstList);
				assert(data != NULL);
				if (isOdd(data)){
					odd--;
				}
				else{
					even--;
				}
				free(data);
			}
		}
		else if (roll == 4){
			printf("POP LAST\n");
			if (listGetSize(firstList) == 0) {
				data = listPopLastElement(firstList);
				assert(data == NULL);
			}
			else{
				data = listPopLastElement(firstList);
				assert(data != NULL);
				if (isOdd(data)){
					odd--;
				}
				else{
					even--;
				}
				free(data);
			}
		}
		else if (roll == 5){
			printf("PUSH FRONT\n");
			assert(listPushFront(firstList, allocateNewEvenNumber()));
			even++;
		}
		else if (roll == 6){
			printf("PUSH FRONT\n");
			assert(listPushFront(firstList, allocateNewOddNumber()));
			odd++;
		}
		else if (roll == 7){
			printf("REMOVE EVEN\n");
			listRemoveElementIf(firstList, free, isEven);
			even = 0;
		}
		else if (roll == 8){
			printf("REMOVE ODD\n");
			listRemoveElementIf(firstList, free, isOdd);
			odd = 0;
		}
		else if (roll == 9){
			printf("ITERATOR GET\n");
			listIterator it;
			listIteratorStart(firstList, &it);
			if (odd + even == 0){
				assert(listIteratorGetFirstElement(&it) == NULL);
			}
			else{
				assert(listIteratorGetLastElement(&it) != NULL);
			}
		}
		else if (roll == 0){
			listIterator it;
			listIteratorStart(firstList, &it);
			if (odd + even == 0){
				assert(listIteratorGetFirstElement(&it) == NULL);
			}
			else{
				printf("REMOVE WITH IT\n");
				int j, max = rand()%(odd+even)/2;
				for (j = 0; j < max; j++){
					listIteratorGetNextElement(&it);
				}
				data = listIteratorGetCurrentElement(&it);
				if(isOdd(data)){
					odd--;
				}
				else{
					even--;
				}

				assert(listIteratorRemoveCurrent(&it, free) != 0);
			}


		}

		assert(listGetSize(firstList) == (odd + even));

	}
	listClear(&firstList, free);
	assert(firstList == NULL);

}

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

	ol = orderedListCreate((int(*)(void*, void*))strcmp);
	assert(orderedListRemoveElement(ol, NULL, "aaaaa") == 0);
	orderedListClear(&ol, NULL);

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

struct testRefpointerStruct_t{
	char *str1, *str2, *str3;
};

static void testRefpointerStructFreeFunction(void* pt){
	struct testRefpointerStruct_t* toFree = (struct testRefpointerStruct_t*) pt;
	referencedPointerUnref(toFree->str1);
	referencedPointerUnref(toFree->str2);
	referencedPointerUnref(toFree->str3);
}

static void testRefpointerRecursiveWithComplexStruct(struct testRefpointerStruct_t* ref, unsigned count){

	if (count == 0){
		return;
	}

	int i = rand() % 4;
	if (i == 0){
		char* x = referencedPointerRef(ref->str1);
		assert(strcmp("string 1", x) == 0);
		testRefpointerRecursiveWithComplexStruct(ref, count - 1);
		referencedPointerUnref(ref->str1);
	}
	else if (i == 1){
		char* x = referencedPointerRef(ref->str2);
		assert(strcmp("string 2", x) == 0);
		testRefpointerRecursiveWithComplexStruct(ref, count - 1);
		referencedPointerUnref(ref->str2);
	}
	else if (i == 2){
		char* x = referencedPointerRef(ref->str3);
		assert(strcmp("string 3", x) == 0);
		testRefpointerRecursiveWithComplexStruct(ref, count - 1);
		referencedPointerUnref(ref->str3);
	}
	else{
		struct testRefpointerStruct_t* x = referencedPointerRef(ref);
		assert(strcmp("string 1", x->str1) == 0);
		assert(strcmp("string 2", x->str2) == 0);
		assert(strcmp("string 3", x->str3) == 0);
		testRefpointerRecursiveWithComplexStruct(ref, count - 1);
		referencedPointerUnref(ref);
	}

}

static const char* testRefpointerString = "Teste com 20 chars.";
static void testRefpointerRecursiveWithString(char* referecendChar, unsigned count){

	if (count == 0){
		return;
	}

	char* x = referencedPointerRef(referecendChar);
	assert(strcmp(x, testRefpointerString) == 0);
	testRefpointerRecursiveWithString(x, count - 1);
	referencedPointerUnref(x);

}

static void testRefpointer(){
	
	char* x = referencedPointerCreate(20 * sizeof(char), NULL);
	snprintf(x, 20, "%s", testRefpointerString);
	srand(time(NULL));
	unsigned recTimes = rand() % 1000;
	printf("Testando ref/unref %u vezes com char*.\n", recTimes);
	testRefpointerRecursiveWithString(x, recTimes);
	assert(strcmp(x, testRefpointerString) == 0);
	assert(referencedPointerUnref(x) == NULL);
	
	struct testRefpointerStruct_t* ref = referencedPointerCreate(sizeof(struct testRefpointerStruct_t), 
			testRefpointerStructFreeFunction);
	
	char* postReferenced = referencedPointerCreate(9 * sizeof(char), NULL);
	snprintf(postReferenced, 9, "string 2");
	
	ref->str1 = referencedPointerCreate(9 * sizeof(char), NULL);
	snprintf(ref->str1, 9, "string 1");
	ref->str2 = referencedPointerRef(postReferenced);
	ref->str3 = referencedPointerCreate(9 * sizeof(char), NULL);
	snprintf(ref->str3, 9, "string 3");
	
	char* toKeep = referencedPointerRef(ref->str1);

	recTimes = rand() % 1000;
	printf("Testando ref/unref %u vezes com struct complexa.\n", recTimes);
	testRefpointerRecursiveWithComplexStruct(ref, recTimes);
	assert(referencedPointerUnref(ref) == NULL);
	
	printf("Ainda deve existir: %s.\n", toKeep);
	assert(referencedPointerUnref(toKeep) == NULL);
	
	printf("Pos referenciado: %s.\n", postReferenced);
	assert(referencedPointerUnref(postReferenced) == NULL);

}

int main(){
	testList();
	//testVector();
	//testOrderedList();
	//testHeap();
	//testPool();
	//testRefpointer();

	return 0;
}
