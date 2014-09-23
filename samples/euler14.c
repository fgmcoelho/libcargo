#include "vector.h"
#include "pool.h"
#include "list.h"
#include <stdio.h>

/*
 * SAMPLE CODE FROM EULER PROJECT:
 *
 * The link to the problem description can be found here:
 * http://projecteuler.net/problem=14
 *
 * This code was created to ilustrate the use of libcargo, if you
 * need the library code, you can find it here:
 * https://github.com/fgmcoelho/libcargo
 *
 * Containers used in this sample:
 * - List.
 * - Pool.
 * - Vector.
 *
 * */

static int collatz_len(vector* registers, pool* numbersPool, long long number){

	list* stack = listCreate();
	int current = 0;
	do{
		if (number < vectorGetSize(registers) && *(long long*)vectorGetElementAt(registers, number) != 0){
			current += *(long long*)vectorGetElementAt(registers, number);
			break;
		}
		else{
			long long *newNumber = poolGetElement(numbersPool);
			*newNumber = number;
			listPushBack(stack, newNumber);
		}

		if ((number % 2) == 0){
			number /= 2;
		}
		else{
			number = (number * 3) + 1;
		}
		current++;
		if (number == 1){
			 current++;
		}
	} while(number != 1);


	long long* stackValue;
	int i;
	for (stackValue = listPopFirstElement(stack), i = 0; stackValue != NULL; stackValue = listPopFirstElement(stack), i++){
		if (*stackValue < vectorGetSize(registers)){
			long long* valueToSet = vectorGetElementAt(registers, *stackValue);
			*valueToSet = current - i;
		}
		poolReturnElement(numbersPool, stackValue);
	}
	listClear(&stack, NULL);

	return current;

}


int main(void){

	pool* numbersPool = poolCreate(1002000, sizeof(long long));
	vector* registers = vectorCreate(1000000);
	long long i;
	for (i = 0; i < 1000000; ++i){
		long long *newElement = poolGetElement(numbersPool);
		*newElement = 0;
		vectorPushBack(registers, newElement);
	}

	long long max = -1, number;
	for (i = 2; i < 1000000; i++){
		long current = collatz_len(registers, numbersPool, i);
		if (current > max){
			max = current;
			number = i;
		}
	}

	printf("Number %lld.\n", number);

	poolClear(&numbersPool);
	vectorClear(&registers, NULL);

	return 0;

}

