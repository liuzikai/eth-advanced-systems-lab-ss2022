//
// Reference：[QuickSort - GeeksforGeeks](https://www.geeksforgeeks.org/quick-sort/)
//

#ifndef _QSORT_D_H
#define _QSORT_D_H

#include "common.h"
#include <stdio.h>
#include "quick_sort.h"

template<class Index>
static inline index_t partition_by_pivot(Index *arr, index_t low, index_t high, Index pivot) {
    index_t i = low;
    index_t j = low;
    for (; j <= high; j++) {
        // If current element is smaller than the pivot
        if (arr[j] > pivot) {
            swap(&arr[i], &arr[j]);
            i++;  // increment index of smaller element
        }
    }
    // swap_decrease(&arr[i], &arr[high]);
    return i;
}

/**
 * Quick sort the array in ascending order.
 * Watch out for negative indexes => uint wrap around. Should we use signed int instead?
 * @param arr     Array to be sorted
 * @param low     Starting index (inclusive)
 * @param high    Ending index (inclusive)
 */
template<class Index>
static inline void quick_cut_sort(Index *arr, index_t low, index_t high, Index u, index_t *count) {
    if (low < high) {
        index_t pi = partition_by_pivot<Index>(arr, low, high, u);
        // Separately sort elements before
        // partition and after partition
        if (pi > 0)
            quick_sort(arr, low, pi - 1);
        *count = pi;
    } else {
        *count = 0;
    }
}

/**
 * Quick sort the array in ascending order.
 * Watch out for negative indexes => uint wrap around. Should we use signed int instead?
 * @param arr     Array to be sorted
 * @param low     Starting index (inclusive)
 * @param high    Ending index (inclusive)
 */
template<class Index>
static inline void quick_cut(Index *arr, index_t low, index_t high, Index u, index_t *count) {
    if (low < high) {
        index_t pi = partition_by_pivot<Index>(arr, low, high, u);
        *count = pi;
    } else {
        *count = 0;
    }
}


#endif //_QSORT_H