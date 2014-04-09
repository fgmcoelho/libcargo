#ifndef __HASHTABLE_H_GUARD__
#define __HASHTABLE_H_GUARD__

typedef struct hash_table_st hashTable;

hashTable* hashTableCreate(unsigned startingSize);
int hashTableInsertElement(hashTable* h, void* keyData, unsigned keySize, void* data);
int hashTableRemoveElement(hashTable* h, void* keyData, unsigned keySize, void (*freeFunction)(void*));
void* hashTableGetElement(hashTable* h, void* keyData, unsigned keySize);
void hashTableClear(hashTable** h, void (*dataFreeFunction)(void*), void (*keyFreeFunction)(void*));

#endif
