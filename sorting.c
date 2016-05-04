#include "sorting.h"

static int partQuicksort(void** indexes, int low, int high, int (*cmp)(const void*, const void*)){
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
        int part = partQuicksort(indexes, low, high, cmp);
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

static void copyArrays(void** original, void** work, int begin, int end){
    int i;
    for (i = begin; i < end; ++i){
        original[i] = work[i];
    }
}

static void mergeArrays(void** original, void** work, int (*cmp)(const void*, const void*), int begin, int mid, int end){
    int i = begin, j = mid, k;
    for (k = begin; k < end; k++){
        if (i < mid && (j >= end || cmp(original[i], original[j]) <= 0)){
            work[k] = original[i];
            i++;
        }
        else{
            work[k] = original[j];
            j++;
        }
    }
}

static void splitMergeSort(void** original, void** work, int (*cmp)(const void*, const void*), int begin, int end){
    if (end - begin < 2){
        return;
    }

    int mid = (end + begin)/2;
    splitMergeSort(original, work, cmp, begin, mid);
    splitMergeSort(original, work, cmp, mid, end);
    mergeArrays(original, work, cmp, begin, mid, end);
    copyArrays(original, work, begin, end);
}

int sortByMergeSort(void** indexes, int size, int (*cmp)(const void*, const void*)){
    if (indexes == NULL || *indexes == NULL || cmp == NULL || size <= 0){
        return 0;
    }

    void** mergeCopy = (void**)malloc(size*sizeof(void*));
    if (mergeCopy == NULL){
        return 0;
    }

    int i;
    for (i = 0; i < size; i++){
        mergeCopy[i] = indexes[i];
    }

    splitMergeSort(indexes, mergeCopy, cmp, 0, size);

    free(mergeCopy);
    return 1;

}
