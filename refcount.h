#ifndef __REFCOUNT_H_GUARD__
#define __REFCOUNT_H_GUARD__

struct referenced_pointer_st;

typedef struct referenced_pointer_st* referencedPointer;

void* referencedPointerCreate(unsigned size, void (*freeFunction)(void*));
void* referencedPointerRef(void* pt);
void* referencedPointerUnref(void* pt);

#endif
