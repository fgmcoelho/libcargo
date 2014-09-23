#include "pool.h"
#include "orderedlist.h"
#include <stdio.h>
#include <string.h>

/*
 * SAMPLE CODE FROM EULER PROJECT:
 *
 * The link to the problem description can be found here:
 * http://projecteuler.net/problem=22
 *
 * This code was created to ilustrate the use of libcargo, if you
 * need the library code, you can find it here:
 * https://github.com/fgmcoelho/libcargo
 *
 * Containers used in this sample:
 * - Ordered List.
 * - Pool.
 *
 * IMPORTANT: The text used here was modified to replace the commas
 * with new lines (0x0A) and remove the double quotes. 
 *
 * */


#define BUFFER_SIZE_TO_USE 32

static unsigned calculateNameValue(char* name){

	unsigned totalSum = 0;
	int i = 0;
	while(name[i] != '\n' && name[i] != '\0'){
		totalSum += name[i] - 'A' + 1;
		i++;
	}
	return totalSum;

}

int main(int argc, char* argv[]){

	pool* namesPool = poolCreate(256, sizeof(char)*BUFFER_SIZE_TO_USE);
	orderedList* namesList = orderedListCreate(256, (int(*)(void*, void*))strcmp);
	
	FILE* fp = fopen("names.txt", "r");
	char line[BUFFER_SIZE_TO_USE], *ret;
	do{
		ret = fgets(line, sizeof(line), fp);
		if (ret != NULL){
			char* newName = poolGetElement(namesPool);
			newName[0] = '\0';
			strncat(newName, line, BUFFER_SIZE_TO_USE - 1);
			orderedListInsertElement(namesList, newName);
		}
	}while (ret != NULL);

	int i;
	int limit = orderedListGetSize(namesList);
	unsigned long long totalSum = 0;
	for (i = 0; i < limit; ++i){
		char* name = orderedListGetElementAt(namesList, i);
		totalSum += (calculateNameValue(name) * (i + 1));
	}

	printf("Total value = %llu.\n", totalSum);
	
	orderedListClear(&namesList, NULL);
	poolClear(&namesPool);
	fclose(fp);

	return 0;

}
