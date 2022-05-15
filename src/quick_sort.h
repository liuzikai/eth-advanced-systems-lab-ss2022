//
// Reference：[QuickSort - GeeksforGeeks](https://www.geeksforgeeks.org/quick-sort/)
//

#ifndef _QSORT_H
#define _QSORT_H

#include "common.h"
#include <stdio.h>

// A utility function to swap two elements
template<class Index>
static inline void swap(Index *a, Index *b) {
    Index t = *a;
    *a = *b;
    *b = t;
}

template<class Index>
static inline index_t partition(Index *arr, index_t low, index_t high) {
    Index pivot = arr[high];  // pivot
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
template<class Index>
static inline void quick_sort(Index *arr, index_t low, index_t high) {
    if (low < high) {

        index_t pi = partition<Index>(arr, low, high);

        // Separately sort elements before
        // partition and after partition
        if (pi > 0)
            quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}

// This is for testing
template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
TRL quick_sort_timing(AdjacencyGraph<Index> *G, void *dummy) {
    (void) dummy;
    // According to sec. 4, the sorting is included in the execution time
    for (Counter u = 0; u < G->n; u++) {
        if (G->adjacency[u].count > 0) {
            quick_sort(G->adjacency[u].neighbors, 0, G->adjacency[u].count - 1);
        }
    }
    TRL res;
    return res;
}

#include <algorithm>

template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
TRL std_sort_timing(AdjacencyGraph<Index> *G, void *dummy) {
    (void) dummy;
    // According to sec. 4, the sorting is included in the execution time
    for (Counter u = 0; u < G->n; u++) {
        if (G->adjacency[u].count > 0) {
            std::sort(G->adjacency[u].neighbors, G->adjacency[u].neighbors + G->adjacency[u].count);
            //quick_sort(G->adjacency[u].neighbors, 0, G->adjacency[u].count - 1);
        }
    }
    TRL res;
    return res;
}

#endif //_QSORT_H
