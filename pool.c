#include "pool.h"

pool* poolCreate(unsigned numberOfElements, unsigned sizeOfElements){

	pool* newPool = (pool*) malloc(sizeof(pool));

	if (newPool == NULL){
		return NULL;
	}

	newPool->memoryBlock = malloc(numberOfElements * sizeOfElements);
	if (newPool->memoryBlock == NULL){
		free(newPool);
		return NULL;
	}
	
	newPool->freeRefs = listCreate();
	if (newPool->freeRefs == NULL){
		free(newPool->memoryBlock);
		free(newPool);
		return NULL;
	}

	int i = 0;
	for (i = 0; i < numberOfElements; ++i){
		if(listAddElement(newPool->freeRefs, (void*)(newPool->memoryBlock+(i*sizeOfElements))) == 0){
			listClear(&newPool->freeRefs, NULL);
			free(newPool->memoryBlock);
			free(newPool);
			return NULL;
		}
	}
	
	newPool->sizeOfElements = sizeOfElements;
	
	return newPool;
}

void* poolGetElement(pool* p){

	if (p == NULL){
		return 0;
	}

	return listPopFirstElement(p->freeRefs);

}

int poolReturnElement(pool* p, void* data){
	if (p == NULL || data == NULL){
		return 0;
	}

	return listAddElement(p->freeRefs, data);

}

void poolClear(pool* p){

	listClear(&p->freeRefs, NULL);
	free(p->memoryBlock);
	free(p);
}

