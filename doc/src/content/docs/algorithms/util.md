---
title: Util
description: Utility functions for common operations
---

The util module provides utility functions for common operations such as swapping elements and generating random numbers. These functions are designed to work with generic data types and are useful building blocks for algorithms and data structures.

## Header Files

To use the utility functions in your code, include the header file:

```c
#include <util/util.h>
```

This provides access to all utility functions.

## Functions

Public interfaces for utility operations:

### util_swap

```c
int util_swap(void * a, void * b, size_t size);
```

Swaps two elements of any type.

**Parameters:**

-   `a` - Pointer to the first element
-   `b` - Pointer to the second element
-   `size` - Size of each element in bytes

**Return Value:**

Returns `0` (`COLLECTION_SUCCESS`) if successful, `-1` (`COLLECTION_FAILURE`) if failed.

**Description:**

Swaps the contents of two memory regions. The function works with any data type by copying bytes between the two locations. The swap operation is performed in-place and modifies both arguments.

**Complexity:** O(1)

**Example:**

```c
#include <util/util.h>
#include <stdio.h>

int main(void) {
    int a = 10, b = 20;
    util_swap(&a, &b, sizeof(int));
    printf("a = %d, b = %d\n", a, b);
    // Prints: a = 20, b = 10

    double x = 3.14, y = 2.71;
    util_swap(&x, &y, sizeof(double));
    printf("x = %f, y = %f\n", x, y);
    // Prints: x = 2.710000, y = 3.140000

    return 0;
}
```

**Notes:**

-   The function swaps the two elements in-place, modifying both arguments
-   If `a` and `b` point to the same memory location, the function returns success without modification
-   The function handles both small and large data types efficiently
-   Passing `NULL` for either pointer or `0` for size will result in failure
-   The function does not validate that the memory regions do not overlap (except when they are the same pointer)

---

### util_random

```c
int util_random(int min, int max);
```

Generates a random integer between min and max (inclusive).

**Parameters:**

-   `min` - The minimum value (inclusive)
-   `max` - The maximum value (inclusive)

**Return Value:**

Returns a random integer in the range [min, max] (inclusive).

**Description:**

Generates a pseudo-random integer within the specified range. The function uses the standard C library random number generator. Both endpoints of the range are inclusive, meaning the result can be equal to either `min` or `max`.

**Complexity:** O(1)

**Example:**

```c
#include <util/util.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    // Seed the random number generator
    srand((unsigned int)time(NULL));

    // Generate random numbers in different ranges
    int dice_roll = util_random(1, 6);
    printf("Dice roll: %d\n", dice_roll);

    int percentage = util_random(0, 100);
    printf("Percentage: %d%%\n", percentage);

    // Generate multiple random numbers
    printf("Random numbers between 10 and 20: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", util_random(10, 20));
    }
    printf("\n");

    return 0;
}
```

**Notes:**

-   The function returns a value in the range [min, max] (both inclusive)
-   If `min` equals `max`, the function returns that value
-   The function works with negative, zero, and positive ranges
-   For best results, seed the random number generator using `srand()` before calling this function
-   The function uses the standard C `rand()` function internally
-   The distribution of random numbers depends on the underlying random number generator implementation

---

## Usage Examples

### Basic Usage

```c
#include <util/util.h>
#include <stdio.h>

int main(void) {
    // Swap integers
    int a = 5, b = 10;
    util_swap(&a, &b, sizeof(int));
    printf("After swap: a = %d, b = %d\n", a, b);

    // Generate random number
    int random_value = util_random(1, 100);
    printf("Random value: %d\n", random_value);

    return 0;
}
```

### Swapping Arrays

```c
#include <util/util.h>
#include <stdio.h>

int main(void) {
    int arr1[] = {1, 2, 3, 4, 5};
    int arr2[] = {6, 7, 8, 9, 10};

    util_swap(arr1, arr2, sizeof(arr1));

    printf("arr1: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", arr1[i]);
    }
    printf("\n");
    // Prints: arr1: 6 7 8 9 10

    printf("arr2: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", arr2[i]);
    }
    printf("\n");
    // Prints: arr2: 1 2 3 4 5

    return 0;
}
```

### Swapping Structures

```c
#include <util/util.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    int id;
    char name[50];
    double score;
} Student;

int main(void) {
    Student student1 = {1, "Alice", 95.5};
    Student student2 = {2, "Bob", 87.0};

    util_swap(&student1, &student2, sizeof(Student));

    printf("Student 1: ID=%d, Name=%s, Score=%.1f\n",
           student1.id, student1.name, student1.score);
    // Prints: Student 1: ID=2, Name=Bob, Score=87.0

    printf("Student 2: ID=%d, Name=%s, Score=%.1f\n",
           student2.id, student2.name, student2.score);
    // Prints: Student 2: ID=1, Name=Alice, Score=95.5

    return 0;
}
```

### Random Array Shuffling

```c
#include <util/util.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    srand((unsigned int)time(NULL));

    int arr[10];
    for (int i = 0; i < 10; i++) {
        arr[i] = i + 1;
    }

    // Shuffle array using Fisher-Yates algorithm
    for (int i = 9; i > 0; i--) {
        int j = util_random(0, i);
        util_swap(&arr[i], &arr[j], sizeof(int));
    }

    printf("Shuffled array: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
```

### Using with Sorting Algorithms

```c
#include <util/util.h>
#include <stdio.h>

void bubble_sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                util_swap(&arr[j], &arr[j + 1], sizeof(int));
            }
        }
    }
}

int main(void) {
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int n = sizeof(arr) / sizeof(arr[0]);

    bubble_sort(arr, n);

    printf("Sorted array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    // Prints: Sorted array: 11 12 22 25 34 64 90

    return 0;
}
```

---

## Complexity

| Operation      | Complexity |
| -------------- | ---------- |
| `util_swap`    | O(1)       |
| `util_random`  | O(1)       |

---

## Important Notes

### util_swap Behavior

-   The function performs a byte-by-byte copy of the specified size
-   For small sizes, the function uses stack-allocated temporary storage
-   For large sizes, the function uses heap-allocated temporary storage
-   Swapping the same pointer with itself is a no-op and returns success
-   The function does not check for overlapping memory regions (except when pointers are identical)

### util_random Behavior

-   The function uses the standard C `rand()` function
-   For reproducible results, seed the random number generator with `srand()` before use
-   The range is inclusive on both ends: [min, max]
-   The function handles edge cases such as min == max correctly
-   The function works with negative, zero, and positive ranges

### Error Handling

**util_swap** returns `COLLECTION_FAILURE` (`-1`) in the following cases:

-   `a` is `NULL`
-   `b` is `NULL`
-   `size` is `0`

**util_random** does not return error codes. However, undefined behavior may occur if:

-   The range is invalid (min > max) - behavior is implementation-dependent
-   The random number generator has not been seeded (results may be predictable)

### Thread Safety

-   **util_swap** is **thread-safe** for swapping different memory regions concurrently
-   **util_random** uses the standard C `rand()` function, which may not be thread-safe on all platforms. For thread-safe random number generation, consider using platform-specific alternatives or synchronization mechanisms.

### Undefined Behavior

Avoid the following:

-   Passing `NULL` pointers to `util_swap()` (returns failure, but should be avoided)
-   Passing overlapping memory regions to `util_swap()` (except when pointers are identical)
-   Passing invalid size to `util_swap()` (size must match the actual data size)
-   Using `util_random()` without seeding the random number generator (results may be predictable)
-   Passing invalid ranges to `util_random()` where min > max (behavior is implementation-dependent)

### Memory Management

-   **util_swap** automatically manages temporary memory allocation
-   For small swaps, no heap allocation occurs
-   For large swaps, heap memory is allocated and freed automatically
-   No manual memory management is required when using `util_swap`

---

## Performance Considerations

### util_swap Performance

-   Constant time complexity O(1) with respect to the number of elements
-   Time complexity is linear with respect to the size of the data being swapped
-   Small data types (less than 512 bytes) use stack allocation for better performance
-   Large data types use heap allocation, which may be slower but handles any size

### util_random Performance

-   Constant time complexity O(1)
-   Very fast operation suitable for high-frequency use
-   Performance depends on the underlying `rand()` implementation
-   No memory allocation is performed

### Best Practices

-   Use `util_swap` for swapping elements in sorting algorithms and data structure operations
-   Seed the random number generator once at program startup when using `util_random`
-   For thread-safe applications, consider using thread-local random number generators or synchronization
-   When swapping large structures frequently, consider if a pointer swap would be more efficient
-   Always ensure the size parameter matches the actual data size to avoid corruption

