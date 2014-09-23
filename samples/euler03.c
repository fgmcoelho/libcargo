#include "list.h"
#include "pool.h"
#include <stdio.h>

/*
 * SAMPLE CODE FROM EULER PROJECT:
 *
 * The link to the problem description can be found here:
 * http://projecteuler.net/problem=3
 *
 * This code was created to ilustrate the use of libcargo, if you
 * need the library code, you can find it here:
 * https://github.com/fgmcoelho/libcargo
 *
 * Containers used in this sample:
 * - List.
 * - Pool.
 *
 * */

void findNextPrime(list* primesList, pool* numbersPool){

	unsigned long long *last = listGetLastElement(primesList);
	if (*last == 2){
		unsigned long long* newElement = poolGetElement(numbersPool);
		*newElement = 3;
		listPushBack(primesList, newElement);
	}
	else{
		int found;
		unsigned long long i;
		unsigned long long start = *last;
		do{
			start += 2;
			found = 1;
			for (i = start/2; i > 1; i--){
				if ((start % i) == 0){
					found = 0;
					break;
				}
			}
			if (found == 1){
				unsigned long long* newElement = poolGetElement(numbersPool);
				*newElement = start;
				listPushBack(primesList, newElement);
			}
		}while(found == 0);
	}

}


int main(int argc, char* argv[]){

	unsigned long long numberToFind = 600851475143L;
	list* primesList = listCreate();
	pool* numbersPool = poolCreate(256, sizeof(unsigned long long));
	if (numbersPool == NULL || primesList == NULL){
		poolClear(&numbersPool);
		listClear(&primesList, NULL);
		printf("Error, out of memory.\n");
		return 1;
	}
	
	unsigned long long firstElement = 2;
	listPushBack(primesList, &firstElement);

	while(numberToFind != 1){
		int found = 0;
		listIterator it;
		listIteratorStart(primesList, &it);
		unsigned long long* current;
		for (current = listIteratorGetFirstElement(&it); current != NULL; current = listIteratorGetNextElement(&it)){
			if ((numberToFind % (*current)) == 0){
				found = 1;
				numberToFind /= (*current);
				break;
			}
		}
		if (found == 0){
			findNextPrime(primesList, numbersPool);
		}
	}

	printf("Biggest prime factor = %llu\n", *(unsigned long long*)(listGetLastElement(primesList)));

	poolClear(&numbersPool);
	listClear(&primesList, NULL);

	return 0;

}
