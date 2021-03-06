#include "refcount.h"
#include <stdlib.h>

struct referenced_pointer_st{
	unsigned count;
	void (*freeFunction)(void*);
};

typedef struct referenced_pointer_st referencedPointerInternal;

#define __REFERENCE_POINTER_JUMP sizeof(referencedPointerInternal)
#ifdef REFERENCED_POINTER_FULL_DEBUG
struct referenced_pointer_log_entry_st {
	char logLine[128-sizeof(void*)-sizeof(int)];
	int state;
	void* which;
};

typedef struct referenced_pointer_log_entry_st logEntry;

#include "orderedlist.h"

static orderedlist* log = NULL;

static int logsCmp(void* a, void* b){
	if (((logEntry*)a)->which > ((logEntry*)b)->which){
		return 1;
	}
	else if (((logEntry*)a)->which == ((logEntry*)b)->which){
		return 0;
	}
	else{
		return -1;
	}
}

static void startLogging(void){
	if (log != NULL){
		return;
	}
	log = orderedListCreate(logsCmp);
}

static void logInsert(referencedPointer newPointer){

}

#endif

void* referencedPointerCreate(unsigned size, void (*freeFunction)(void*)){
	if (size == 0){
		return NULL;
	}

	void *data = malloc(size + __REFERENCE_POINTER_JUMP);
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

