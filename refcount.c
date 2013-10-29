#include "refcount.h"
#include <stdlib.h>

#define __REFERENCE_POINTER_JUMP sizeof(referencedPointerInternal)

void* referencedPointerCreate(unsigned size, void (*freeFunction)(void*)){
	if (size == 0){
		return NULL;
	}
	
	void *data = malloc(size + sizeof(referencedPointerInternal));
	if (data == NULL){
		return NULL;
	}

	referencedPointer newReferencedPointer = (referencedPointer)data;
	newReferencedPointer->count = 1;
	newReferencedPointer->freeFunction = freeFunction;

	return (data + sizeof(referencedPointerInternal));
}

void* referencedPointerRef(void* pt){
	referencedPointer refPt = (referencedPointer)(pt - __REFERENCE_POINTER_JUMP);
	refPt->count++;

	return pt;
}

void* referencedPointerUnref(void* pt){
	referencedPointer refPt = (referencedPointer)(pt - __REFERENCE_POINTER_JUMP);
	if(refPt->count == 1){
		if (refPt->freeFunction != NULL){
			refPt->freeFunction(pt);
		}
		free(refPt);
		return NULL;
	}
	else{
		refPt->count--;
		return pt;
	}
}

