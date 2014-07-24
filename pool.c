#include "pool.h"

#include <stdio.h>

struct pool_st {
	unsigned sizeOfElements, elementsPerBlock;
	list *freeRefs, *memoryBlocks;;
};


pool* poolCreate(unsigned numberOfElements, unsigned sizeOfElements){

	pool* newPool = (pool*) malloc(sizeof(pool));

	if (newPool == NULL){
		return NULL;
	}

	newPool->memoryBlocks = listCreate();	
	if (newPool->memoryBlocks == NULL){
		free(newPool);
		return NULL;
	}
	
	void* currentBlock = malloc(numberOfElements * sizeOfElements);
	if (currentBlock == NULL){
		listClear(&newPool->memoryBlocks, NULL);
		return NULL;
	}


	newPool->freeRefs = listCreate();
	if (newPool->freeRefs == NULL){
		listClear(&newPool->memoryBlocks, NULL);
		free(currentBlock);
		free(newPool);
		return NULL;
	}

	if(listPushBack(newPool->memoryBlocks, currentBlock) == 0){
		listClear(&newPool->memoryBlocks, NULL);
		free(currentBlock);
		free(newPool);
		return NULL;
	}

	unsigned i = 0;
	for (i = 0; i < numberOfElements; ++i){
		if(listPushBack(newPool->freeRefs, (void*)(currentBlock + (i * sizeOfElements))) == 0){
			listClear(&newPool->freeRefs, NULL);
			listClear(&newPool->memoryBlocks, NULL);
			free(currentBlock);
			free(newPool);
			return NULL;
		}
	}
	
	newPool->sizeOfElements = sizeOfElements;
	newPool->elementsPerBlock = numberOfElements;

	return newPool;
}

void* poolGetElement(pool* p){

	if (p == NULL){
		return 0;
	}

	void* elementToReturn = listPopFirstElement(p->freeRefs);
	if (elementToReturn != NULL){
		return elementToReturn;
	}

	void* newBlock = malloc(p->sizeOfElements * p->elementsPerBlock);
	if (newBlock == NULL){
		return NULL;
	}

	list* tmpList = listCreate();
	if (tmpList == NULL){
		free(newBlock);
		return NULL;
	}

	unsigned i = 0;
	for (i = 0; i < p->elementsPerBlock; ++i){
		if(listPushBack(tmpList, (void*)(newBlock + (i * p->sizeOfElements))) == 0){
			listClear(&tmpList, NULL);
			free(newBlock);
			return NULL;
		}
	}

	if (listPushBack(p->memoryBlocks, newBlock) == 0){
			listClear(&tmpList, NULL);
			free(newBlock);
			return NULL;
	}

	if (listMergeLists(&p->freeRefs, &tmpList) == 0){
			listClear(&tmpList, NULL);
			free(newBlock);
			return NULL;
	}

	return listPopFirstElement(p->freeRefs);

}

int poolReturnElement(pool* p, void* data){
	if (p == NULL || data == NULL){
		return 0;
	}

	return listPushBack(p->freeRefs, data);

}

void poolClear(pool** p){

	if (p == NULL || *p == NULL){
		return;
	}

	listClear(&(*p)->freeRefs, NULL);
	listClear(&(*p)->memoryBlocks, free);
	free(*p);

	*p = NULL;
}

