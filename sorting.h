#ifndef __SORTING_H_GUARD__
#define __SORTING_H_GUARD__
#include <stdlib.h>

int sortByQuickSort(void** indexes, int size, int (*cmp)(const void*, const void*));
int sortByMergeSort(void** indexes, int size, int (*cmp)(const void*, const void*));

#endif
