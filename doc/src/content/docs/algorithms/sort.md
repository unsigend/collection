---
title: Sort
description: Sorting algorithms for generic data types
---

The sort module provides efficient sorting algorithms for generic data types. The algorithms work with any data type by using a comparison function, making them suitable for sorting arrays of integers, floating-point numbers, strings, structures, and other custom types.

## Header Files

To use the sorting functions in your code, include the header file:

```c
#include <algorithm/sort.h>
```

This provides access to all sorting functions.

## Functions

Public interfaces for sorting operations:

### sort_insertion

```c
int sort_insertion(void * data, size_t n, size_t size,
    int (*compare)(const void *, const void *));
```

Sorts an array using the insertion sort algorithm.

**Parameters:**

-   `data` - Pointer to the array to sort
-   `n` - Number of elements in the array
-   `size` - Size of each element in bytes
-   `compare` - Comparison function that returns negative if first argument is less than second, zero if equal, positive if greater

**Return Value:**

Returns `0` (`COLLECTION_SUCCESS`) if successful, `-1` (`COLLECTION_FAILURE`) if failed.

**Description:**

Sorts the array in-place using insertion sort. The algorithm is stable and works well for small arrays or arrays that are already partially sorted.

**Complexity:** O(n²)

**Example:**

```c
#include <algorithm/sort.h>
#include <stdio.h>

int compare_int(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    if (ia < ib) return -1;
    if (ia > ib) return 1;
    return 0;
}

int main(void) {
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    size_t n = sizeof(arr) / sizeof(arr[0]);

    sort_insertion(arr, n, sizeof(int), compare_int);

    for (size_t i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    // Prints: 11 12 22 25 34 64 90
    return 0;
}
```

**Notes:**

-   The function sorts the array in-place, modifying the original array
-   Empty arrays (n = 0) are handled correctly and return success
-   Single-element arrays are already sorted and return success
-   The comparison function must not be `NULL`
-   The data pointer must not be `NULL` (unless n is 0)

---

### sort_selection

```c
int sort_selection(void * data, size_t n, size_t size,
    int (*compare)(const void *, const void *));
```

Sorts an array using the selection sort algorithm.

**Parameters:**

-   `data` - Pointer to the array to sort
-   `n` - Number of elements in the array
-   `size` - Size of each element in bytes
-   `compare` - Comparison function that returns negative if first argument is less than second, zero if equal, positive if greater

**Return Value:**

Returns `0` (`COLLECTION_SUCCESS`) if successful, `-1` (`COLLECTION_FAILURE`) if failed.

**Description:**

Sorts the array in-place using selection sort. The algorithm repeatedly finds the minimum element from the unsorted portion and places it at the beginning.

**Complexity:** O(n²)

**Example:**

```c
#include <algorithm/sort.h>
#include <stdio.h>

int compare_int(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    if (ia < ib) return -1;
    if (ia > ib) return 1;
    return 0;
}

int main(void) {
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    size_t n = sizeof(arr) / sizeof(arr[0]);

    sort_selection(arr, n, sizeof(int), compare_int);

    for (size_t i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    // Prints: 11 12 22 25 34 64 90
    return 0;
}
```

**Notes:**

-   The function sorts the array in-place, modifying the original array
-   Empty arrays (n = 0) are handled correctly and return success
-   Single-element arrays are already sorted and return success
-   The comparison function must not be `NULL`
-   The data pointer must not be `NULL` (unless n is 0)

---

### sort_bubble

```c
int sort_bubble(void * data, size_t n, size_t size,
    int (*compare)(const void *, const void *));
```

Sorts an array using the bubble sort algorithm.

**Parameters:**

-   `data` - Pointer to the array to sort
-   `n` - Number of elements in the array
-   `size` - Size of each element in bytes
-   `compare` - Comparison function that returns negative if first argument is less than second, zero if equal, positive if greater

**Return Value:**

Returns `0` (`COLLECTION_SUCCESS`) if successful, `-1` (`COLLECTION_FAILURE`) if failed.

**Description:**

Sorts the array in-place using bubble sort. The algorithm repeatedly steps through the array, compares adjacent elements and swaps them if they are in the wrong order.

**Complexity:** O(n²)

**Example:**

```c
#include <algorithm/sort.h>
#include <stdio.h>

int compare_int(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    if (ia < ib) return -1;
    if (ia > ib) return 1;
    return 0;
}

int main(void) {
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    size_t n = sizeof(arr) / sizeof(arr[0]);

    sort_bubble(arr, n, sizeof(int), compare_int);

    for (size_t i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    // Prints: 11 12 22 25 34 64 90
    return 0;
}
```

**Notes:**

-   The function sorts the array in-place, modifying the original array
-   Empty arrays (n = 0) are handled correctly and return success
-   Single-element arrays are already sorted and return success
-   The comparison function must not be `NULL`
-   The data pointer must not be `NULL` (unless n is 0)

---

### sort_quick

```c
int sort_quick(void * data, size_t n, size_t size,
    int (*compare)(const void *, const void *));
```

Sorts an array using the quicksort algorithm.

**Parameters:**

-   `data` - Pointer to the array to sort
-   `n` - Number of elements in the array
-   `size` - Size of each element in bytes
-   `compare` - Comparison function that returns negative if first argument is less than second, zero if equal, positive if greater

**Return Value:**

Returns `0` (`COLLECTION_SUCCESS`) if successful, `-1` (`COLLECTION_FAILURE`) if failed.

**Description:**

Sorts the array in-place using quicksort. The algorithm uses a divide-and-conquer approach, partitioning the array around a pivot element and recursively sorting the sub-arrays.

**Complexity:** O(n log n)

**Example:**

```c
#include <algorithm/sort.h>
#include <stdio.h>

int compare_int(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    if (ia < ib) return -1;
    if (ia > ib) return 1;
    return 0;
}

int main(void) {
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    size_t n = sizeof(arr) / sizeof(arr[0]);

    sort_quick(arr, n, sizeof(int), compare_int);

    for (size_t i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    // Prints: 11 12 22 25 34 64 90
    return 0;
}
```

**Notes:**

-   The function sorts the array in-place, modifying the original array
-   Empty arrays (n = 0) are handled correctly and return success
-   Single-element arrays are already sorted and return success
-   The comparison function must not be `NULL`
-   The data pointer must not be `NULL` (unless n is 0)

---

### sort_merge

```c
int sort_merge(void * data, size_t n, size_t size,
    int (*compare)(const void *, const void *));
```

Sorts an array using the merge sort algorithm.

**Parameters:**

-   `data` - Pointer to the array to sort
-   `n` - Number of elements in the array
-   `size` - Size of each element in bytes
-   `compare` - Comparison function that returns negative if first argument is less than second, zero if equal, positive if greater

**Return Value:**

Returns `0` (`COLLECTION_SUCCESS`) if successful, `-1` (`COLLECTION_FAILURE`) if failed.

**Description:**

Sorts the array in-place using merge sort. The algorithm uses a divide-and-conquer approach, recursively dividing the array into smaller sub-arrays, sorting them, and then merging them back together. Merge sort is stable and provides guaranteed O(n log n) performance in all cases.

**Complexity:** O(n log n)

**Example:**

```c
#include <algorithm/sort.h>
#include <stdio.h>

int compare_int(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    if (ia < ib) return -1;
    if (ia > ib) return 1;
    return 0;
}

int main(void) {
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    size_t n = sizeof(arr) / sizeof(arr[0]);

    sort_merge(arr, n, sizeof(int), compare_int);

    for (size_t i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    // Prints: 11 12 22 25 34 64 90
    return 0;
}
```

**Notes:**

-   The function sorts the array in-place, modifying the original array
-   Empty arrays (n = 0) are handled correctly and return success
-   Single-element arrays are already sorted and return success
-   The comparison function must not be `NULL`
-   The data pointer must not be `NULL` (unless n is 0)

---

## Usage Examples

### Basic Usage

```c
#include <algorithm/sort.h>
#include <stdio.h>

int compare_int(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    if (ia < ib) return -1;
    if (ia > ib) return 1;
    return 0;
}

int main(void) {
    int arr[] = {5, 2, 8, 1, 3};
    size_t n = sizeof(arr) / sizeof(arr[0]);

    sort_insertion(arr, n, sizeof(int), compare_int);

    for (size_t i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    // Prints: 1 2 3 5 8
    return 0;
}
```

### Sorting Strings

```c
#include <algorithm/sort.h>
#include <stdio.h>
#include <string.h>

int compare_string(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int main(void) {
    const char *arr[] = {"banana", "apple", "cherry", "date"};
    size_t n = sizeof(arr) / sizeof(arr[0]);

    sort_insertion(arr, n, sizeof(char *), compare_string);

    for (size_t i = 0; i < n; i++) {
        printf("%s ", arr[i]);
    }
    // Prints: apple banana cherry date
    return 0;
}
```

---

## Complexity

| Operation        | Complexity |
| ---------------- | ---------- |
| `sort_insertion` | O(n²)      |
| `sort_selection` | O(n²)      |
| `sort_bubble`    | O(n²)      |
| `sort_quick`     | O(n log n) |
| `sort_merge`     | O(n log n) |

Where n is the number of elements to sort.

---

## Important Notes

### Comparison Function

All sorting functions use the same comparison function interface, following the standard C library `qsort` convention:

-   Return a negative value if the first argument should come before the second
-   Return zero if the arguments are equal
-   Return a positive value if the first argument should come after the second

### Common Behavior

All sorting functions share the following characteristics:

-   Functions sort the array in-place, modifying the original array
-   Empty arrays (n = 0) are handled correctly and return success
-   Single-element arrays are already sorted and return success
-   The comparison function must not be `NULL`
-   The data pointer must not be `NULL` (unless n is 0)

### Error Handling

All sorting functions return `COLLECTION_FAILURE` (`-1`) in the following cases:

-   `data` is `NULL` and `n` is greater than 0
-   `compare` is `NULL`
-   `size` is 0

### Thread Safety

Sorting functions are **not thread-safe** if multiple threads access the same array concurrently. However, different threads can safely sort different arrays simultaneously.

### Undefined Behavior

Avoid the following:

-   Passing `NULL` as `data` when `n` is greater than 0
-   Passing `NULL` as `compare`
-   Passing `size` as 0
-   Modifying the array during sorting
-   Accessing array elements outside the valid range [0, n-1] during sorting

---

## Performance Considerations

### General Guidelines

When choosing a sorting algorithm, consider:

-   **Array size:** Some algorithms perform better on small arrays, others on large arrays
-   **Data distribution:** Some algorithms are adaptive and perform well on partially sorted data
-   **Stability:** Whether equal elements must maintain their relative order
-   **Memory constraints:** Some algorithms require additional memory proportional to array size
-   **Worst-case performance:** Consider worst-case scenarios for your use case

### Memory Usage

-   All sorting algorithms use O(1) additional space complexity
-   Temporary memory used during sorting is automatically managed

### Algorithm Selection

Different algorithms are optimized for different scenarios:

-   **Small arrays:** Simple algorithms like insertion sort may be faster due to low overhead
-   **Large arrays:** More complex algorithms like quicksort or merge sort typically perform better
-   **Partially sorted data:** Adaptive algorithms can take advantage of existing order
-   **Stability requirements:** Choose a stable algorithm if relative order of equal elements matters
-   **Worst-case guarantees:** Some algorithms provide guaranteed worst-case performance bounds
