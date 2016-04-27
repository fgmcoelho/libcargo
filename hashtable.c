#include "hashtable.h"
#include "pool.h"

#include <stdio.h>

#define HASH_TABLE_MAX_COLLISIONS 3
#define HASH_TABLE_RESIZE_DOUBLE  2

static unsigned char byteMapTable[256] = {
	238,  93,  78, 153, 181,  80,  29,  94, 223, 197, 145,  35, 104, 130, 201,  23,
	137, 198, 162,  51,  72,   1, 248, 117,  24,  34, 195,  41,  44, 175, 186, 204,
	245,  48,   3,  55,  27,  52, 193, 194,   5, 124,  84,  30,  12,  75,  81, 212,
	 89,  95,  22, 180, 146,  65, 165,  31, 235, 206, 149, 185, 214,  43, 166, 200,
	140, 102, 188, 161,  11, 125, 196, 230,  13,  67,  19, 101, 237, 211,  21,   0,
	189, 218, 239, 105, 232,   9, 252, 203, 112, 229,  76, 182, 141, 136, 176, 231,
	 40, 199, 143, 250, 191,  90,  45,  33,  86,  56,  37, 213,  15, 148, 172, 174,
	100, 126,   6, 225, 253,  10, 132,  74,  77, 168, 208,  98,  79, 127, 157, 133,
	113,  99, 219,   4, 114, 190, 207, 163, 187,  57,  59,  70,  36, 205,  96, 224,
	183,  63, 103, 173, 177,  73,  17,  66, 249,  68, 151, 228,   7, 255, 254, 108,
	247,  62, 192, 159,  47,  85,  20,  16, 222,  39,  88, 170, 107, 110, 169, 122,
	109, 115,  18, 210, 241, 236, 202, 178, 116, 106, 160, 251,  14, 111, 234, 221,
	 38, 242,  42, 244, 217, 156, 129,  53, 131, 179,  32, 119, 215, 220, 134,  28,
	138, 152, 142,  26, 167, 155, 227, 209, 216,  60,  25, 184,  69,  61,  97,  58,
	120,  71,  64, 226, 240,  91,   2,  50,  49, 171, 135, 154, 233, 147,  82, 144,
	123,  83, 243, 246, 128, 164,  54, 139,  87,   8, 121,  46, 158, 118,  92, 150
};

struct hash_key_st{
	void* bytes;
	unsigned size;
};

typedef struct hash_key_st hashKey;

struct hash_entry_st {
	void* data;
	hashKey* key;
	struct hash_entry_st* next;
};

typedef struct hash_entry_st hashEntry;

struct hash_table_st{
	unsigned long size;
	unsigned long used;
	unsigned long resizeAt;
	unsigned maxCollision;
	pool* keysPool;
	pool* entriesPool;
	hashEntry** info;
};

static unsigned long hashTableCalculateIndex(void* keyData, unsigned size, unsigned long modToUse){

	unsigned long indexHash = 0;
	unsigned i, j;

	unsigned char firstByte = *(unsigned char*)keyData, hashByte;
	for (i = 0; i < sizeof(unsigned long); ++i){
		hashByte = byteMapTable[0 ^ firstByte];
		for (j = 1; j < size; ++j){
			hashByte = byteMapTable[hashByte ^ (((unsigned char*)keyData)[j])];

		}
		((unsigned char*)&indexHash)[i] = hashByte;
		firstByte++;
	}

	return indexHash % modToUse;

}

static inline int hashKeyCompare(hashKey* first, hashKey* second){

	if (first->size != second->size){
		return 0;
	}

	unsigned i;
	for (i = 0; i < first->size; ++i){
		if (((unsigned char*)(first->bytes))[i] != ((unsigned char*)(second->bytes))[i]){
			return 0;
		}
	}

	return 1;

}

static int hashTableSlotContains(hashEntry* base, hashEntry* toSearch){

	while(base != NULL){
		if(hashKeyCompare(base->key, toSearch->key) == 1){
			return 1;
		}
		base = base->next;
	}

	return 0;

}

static int hashTableFindIndexAndInsert(hashEntry** entries, hashEntry* entryToUse, unsigned* collisions, unsigned long index){

	*collisions = 0;
	if (entries[index] != NULL){
		if (hashTableSlotContains(entries[index], entryToUse) == 1){
			return 0;
		}
		else{
			hashEntry* aux = entries[index];
			unsigned i = 0;
			do{
				if (aux->next == NULL){
					aux->next = entryToUse;
					*collisions = *collisions + 1;
					i++;
					break;
				}
				else{
					aux = aux->next;
				}
				i++;
			} while (aux != NULL);
		}
	}
	else{
		entries[index] = entryToUse;
	}

	return 1;
}

static int hashTableResize(hashTable* h, unsigned sizeFactor){

	unsigned long newSize = h->size * sizeFactor;
	hashEntry** newInfo = malloc (sizeof(hashEntry*) * (newSize));
	if (newInfo == NULL){
		return 0;
	}

	unsigned long i;
	unsigned collisionsCount, maxCollisions = 0;
	for (i = 0; i < newSize; ++i){
		newInfo[i] = NULL;
	}

	for (i = 0; i < h->size; ++i){
		if (h->info[i] != NULL){
			hashEntry* aux = h->info[i], *next;

			while (aux != NULL){
				unsigned long index = hashTableCalculateIndex(aux->key->bytes, aux->key->size, newSize);
				hashTableFindIndexAndInsert(newInfo, aux, &collisionsCount, index);
				next = aux->next;
				aux->next = NULL;
				if (maxCollisions < collisionsCount){
					maxCollisions = collisionsCount;
				}
				aux = next;
			}
		}
	}

	h->size = newSize;
	free(h->info);
	h->info = newInfo;
	h->resizeAt = (newSize * 3) / 4;

	if (maxCollisions > HASH_TABLE_MAX_COLLISIONS){
		return hashTableResize(h, HASH_TABLE_RESIZE_DOUBLE);
	}

	return 1;
}

hashTable* hashTableCreate(unsigned startingSize){

	hashTable* newHashTable = malloc(sizeof(hashTable));
	if (newHashTable == NULL){
		return NULL;
	}

	newHashTable->info = malloc (sizeof(hashEntry*) * startingSize);
	if (newHashTable->info == NULL){
		free(newHashTable);
		return NULL;
	}

	newHashTable->keysPool = poolCreate(startingSize, sizeof(hashKey));
	if(newHashTable->keysPool == NULL){
		free(newHashTable->info);
		free(newHashTable);
		return NULL;
	}

	newHashTable->entriesPool = poolCreate(startingSize, sizeof(hashEntry));
	if (newHashTable->entriesPool == NULL){
		free(newHashTable->info);
		poolClear(&newHashTable->keysPool);
		free(newHashTable);
	}

	newHashTable->size = startingSize;
	newHashTable->maxCollision = 0;
	newHashTable->used = 0;
	newHashTable->resizeAt = (startingSize * 3) / 4;

	unsigned long i;
	for (i = 0; i < startingSize; ++i){
		newHashTable->info[i] = NULL;
	}

	return newHashTable;

}

int hashTableInsertElement(hashTable* h, void* keyData, unsigned keySize, void* data){

	if (h == NULL || keyData == NULL || keySize == 0 || data == NULL){
		return 0;
	}

	if (h->used == h->resizeAt && hashTableResize(h, HASH_TABLE_RESIZE_DOUBLE) == 0){
		return 0;
	}

	hashKey* newKey = poolGetElement(h->keysPool);
	if (newKey == NULL){
		return 0;
	}

	hashEntry* newEntry = poolGetElement(h->entriesPool);
	if (newEntry == NULL){
		return 0;
	}

	unsigned long index = hashTableCalculateIndex(keyData, keySize, h->size);
	newKey->bytes = keyData;
	newKey->size = keySize;

	newEntry->key = newKey;
	newEntry->data = data;
	newEntry->next = NULL;

	unsigned collisionsCount;
	if(hashTableFindIndexAndInsert(h->info, newEntry, &collisionsCount, index) == 0){
		poolReturnElement(h->keysPool, newKey);
		poolReturnElement(h->entriesPool, newEntry);
		return 1;
	}

	h->used++;

	if (collisionsCount > h->maxCollision){
		h->maxCollision = collisionsCount;
	}

	if (h->maxCollision > HASH_TABLE_MAX_COLLISIONS){
		return hashTableResize(h, HASH_TABLE_RESIZE_DOUBLE);
	}

	return 1;

}

int hashTableRemoveElement(hashTable* h, void* keyData, unsigned keySize, void (*freeFunction)(void*)){

	if (h == NULL || keyData == NULL || keySize == 0){
		return 0;
	}

	hashKey searchingKey;
	searchingKey.size = keySize;
	searchingKey.bytes = keyData;

	unsigned long index = hashTableCalculateIndex(keyData, keySize, h->size);

	hashEntry* base = h->info[index], *former = NULL;
	while(base != NULL){
		if(hashKeyCompare(base->key, &searchingKey) == 1){
			if (base == h->info[index]){
				h->info[index] = base->next;
			}
			else{
				former->next = base->next;
			}
			poolReturnElement(h->keysPool, base->key);
			poolReturnElement(h->entriesPool, base);
			if (freeFunction != NULL){
				freeFunction(base->data);
			}
			return 1;
		}
		former = base;
		base = base->next;
	}

	return 0;

}

void* hashTableGetElement(hashTable* h, void* keyData, unsigned keySize){

	if (h == NULL || keyData == NULL || keySize == 0){
		return NULL;
	}

	hashKey searchingKey;
	searchingKey.size = keySize;
	searchingKey.bytes = keyData;

	unsigned long index = hashTableCalculateIndex(keyData, keySize, h->size);


	hashEntry* base = h->info[index];
	while(base != NULL){
		if(hashKeyCompare(base->key, &searchingKey) == 1){
			return base->data;
		}
		base = base->next;
	}

	return NULL;

}

void hashTableClear(hashTable** h, void (*dataFreeFunction)(void*), void (*keyFreeFunction)(void*)){

	if (h == NULL || *h == NULL){
		return;
	}

	if (dataFreeFunction != NULL){
		unsigned long i;
		for (i = 0; i < (*h)->size; i++){
			if ((*h)->info[i] != NULL){
				hashEntry* entry = (*h)->info[i], *aux;
				while(entry != NULL){
					aux = entry->next;
					if (dataFreeFunction != NULL){
						dataFreeFunction(entry->data);
					}
					if (keyFreeFunction != NULL){
						keyFreeFunction(entry->key->bytes);
					}
					entry = aux;
				}
			}
		}
	}

	poolClear(&((*h)->keysPool));
	poolClear(&((*h)->entriesPool));
	free((*h)->info);
	free((*h));

	(*h) = NULL;
}
