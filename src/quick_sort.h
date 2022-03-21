//
// Created by Zikai Liu on 12/11/19.
// Reference：[QuickSort - GeeksforGeeks](https://www.geeksforgeeks.org/quick-sort/)
//

#ifndef _QSORT_H
#define _QSORT_H

#include "common.h"
#include <stdio.h>

// A utility function to swap two elements
static inline void swap(index_t *a, index_t *b) {
    index_t t = *a;
    *a = *b;
    *b = t;
}

static inline index_t partition(index_t *arr, index_t low, index_t high) {
    index_t pivot = arr[high];  // pivot
    index_t i = low;
    index_t j = low;
    for (; j <= high - 1; j++) {
        // If current element is smaller than the pivot
        if (arr[j] < pivot) {
            swap(&arr[i], &arr[j]);
            i++;  // increment index of smaller element
        }
    }
    swap(&arr[i], &arr[high]);
    return i;
}

/**
 * Quick sort the array in ascending order.
 * Watch out for negative indexes => uint wrap around. Should we use signed int instead?
 * @param arr     Array to be sorted
 * @param low     Starting index (inclusive)
 * @param high    Ending index (inclusive)
 */
static inline void quick_sort(index_t *arr, index_t low, index_t high) {
    if (low < high) {

        index_t pi = partition(arr, low, high);

        // Separately sort elements before
        // partition and after partition
        if (pi > 0)
            quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}

#endif //_QSORT_H
