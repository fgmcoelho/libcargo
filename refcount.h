#ifndef __REFCOUNT_H_GUARD__
#define __REFCOUNT_H_GUARD__

struct referenced_pointer_t{
	unsigned count;
	void (*freeFunction)(void*);
};

typedef struct referenced_pointer_t referencedPointerInternal;
typedef struct referenced_pointer_t* referencedPointer;

void* referencedPointerCreate(unsigned size, void (*freeFunction)(void*));
void* referencedPointerRef(void* pt);
void* referencedPointerUnref(void* pt);

#endif
