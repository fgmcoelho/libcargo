#include "sorting.h"

static int part_quicksort(void** indexes, int low, int high, int (*cmp)(const void*, const void*)){
    void *swap, *pivot = indexes[high];
    int j, i = low;
    for (j = low; j < high; j++){
        if (cmp(indexes[j], pivot) <= 0){
            swap = indexes[i];
            indexes[i] = indexes[j];
            indexes[j] = swap;
            i++;
        }
    }
    swap = indexes[i];
    indexes[i] = indexes[high];
    indexes[high] = swap;

    return i;
}

static void quicksort(void** indexes, int low, int high, int (*cmp)(const void*, const void*)){
    if (low < high){
        int part = part_quicksort(indexes, low, high, cmp);
        quicksort(indexes, low, part - 1, cmp);
        quicksort(indexes, part + 1, high, cmp);
    }

}

int sortByQuickSort(void** indexes, int size, int (*cmp)(const void*, const void*)){
    if (indexes == NULL || *indexes == NULL || cmp == NULL || size <= 0){
        return 0;
    }
    quicksort(indexes, 0, size, cmp);
    return 1;
}

