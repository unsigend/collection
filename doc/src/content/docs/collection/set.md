---
title: Set
description: Unordered collection of unique elements
---

The `Set` is an unordered collection of unique elements that provides efficient insertion, deletion, and membership testing using a hash table implementation.

## Header Files

To use the set in your code, include the header file:

```c
#include <collection/set.h>
```

This provides access to the `Set` type and all set functions.

## Functions

Public interfaces for set operations:

### set_init

```c
int set_init(Set* set,
    uint32_t (*hash)(const void * key),
    bool (*match)(const void * key1, const void * key2),
    void (*destroy)(void *));
```

Initializes a set to an empty state.

**Parameters:**

-   `set` - Pointer to the set to initialize
-   `hash` - Hash function for elements. Pass `NULL` to use default string hash function
-   `match` - Match function to compare elements. Pass `NULL` to use default string comparison
-   `destroy` - Optional destructor function called when elements are removed. Pass `NULL` if no cleanup is needed

**Return Value:**

Returns `0` on success, `-1` on failure.

**Complexity:** O(1)

**Example:**

```c
Set set;
set_init(&set, NULL, NULL, NULL);  // Use defaults

// With custom hash and match functions
set_init(&set, hash_str,
    (bool (*)(const void *, const void *))strcmp, NULL);

// With destroy function
set_init(&set, NULL, NULL, free);
```

---

### set_destroy

```c
void set_destroy(Set* set);
```

Destroys the set and frees all associated memory.

**Parameters:**

-   `set` - Pointer to the set to destroy

**Description:**

Calls the destroy function (if provided) on all elements, then frees all resources. After destruction, the set is reset to an empty state.

**Complexity:** O(n) where n is the number of elements

**Example:**

```c
Set set;
set_init(&set, NULL, NULL, NULL);
// ... use set ...
set_destroy(&set);
```

---

### set_empty

```c
bool set_empty(const Set* set);
```

Checks if the set is empty.

**Parameters:**

-   `set` - Pointer to the set

**Return Value:**

`true` if the set is empty, `false` otherwise. Returns `true` if `set` is `NULL`.

**Complexity:** O(1)

**Example:**

```c
Set set;
set_init(&set, NULL, NULL, NULL);

if (set_empty(&set)) {
    printf("Set is empty\n");
}

set_insert(&set, "element");
if (!set_empty(&set)) {
    printf("Set is not empty\n");
}
```

---

### set_size

```c
size_t set_size(const Set* set);
```

Returns the number of elements in the set.

**Parameters:**

-   `set` - Pointer to the set

**Return Value:**

The number of elements currently stored in the set. Returns `0` if `set` is `NULL`.

**Complexity:** O(1)

**Example:**

```c
Set set;
set_init(&set, NULL, NULL, NULL);

printf("Size: %zu\n", set_size(&set));  // Prints "Size: 0"

set_insert(&set, "element1");
set_insert(&set, "element2");
printf("Size: %zu\n", set_size(&set));  // Prints "Size: 2"
```

---

### set_insert

```c
int set_insert(Set* set, const void * data);
```

Inserts an element into the set.

**Parameters:**

-   `set` - Pointer to the set
-   `data` - Pointer to the element to insert

**Return Value:**

Returns `0` on success, `-1` on failure.

**Description:**

If the element already exists, the insertion is ignored and the size of the set does not increase. If memory allocation fails, `-1` is returned and the set remains unchanged.

**Complexity:** O(1) average case

**Example:**

```c
Set set;
set_init(&set, NULL, NULL, NULL);

if (set_insert(&set, "element1") == 0) {
    // Successfully inserted
}

// Inserting duplicate element
set_insert(&set, "element1");  // Size remains 1
```

---

### set_remove

```c
int set_remove(Set* set, const void * data);
```

Removes an element from the set.

**Parameters:**

-   `set` - Pointer to the set
-   `data` - The element to remove

**Return Value:**

Returns `0` on success, `-1` on failure (e.g., element not found).

**Description:**

Removes the element from the set. The destroy function is called on the element if provided during initialization.

**Complexity:** O(1) average case

**Example:**

```c
Set set;
set_init(&set, NULL, NULL, NULL);

set_insert(&set, "element1");
set_insert(&set, "element2");

if (set_remove(&set, "element1") == 0) {
    // Successfully removed
}

// Remove non-existing element
if (set_remove(&set, "nonexistent") == -1) {
    // Element not found
}
```

---

### set_clear

```c
void set_clear(Set* set);
```

Removes all elements from the set.

**Parameters:**

-   `set` - Pointer to the set

**Description:**

Removes all elements from the set and sets size to 0. The destroy function is called on each element if provided. After clearing, the set is in an empty state and can be reused.

**Complexity:** O(n) where n is the number of elements

**Example:**

```c
Set set;
set_init(&set, NULL, NULL, NULL);

set_insert(&set, "element1");
set_insert(&set, "element2");
set_insert(&set, "element3");
// size=3

set_clear(&set);
// size=0

// Set can be reused
set_insert(&set, "newelement");
// size=1
```

---

### set_contains

```c
bool set_contains(const Set* set, const void * data);
```

Checks if an element exists in the set.

**Parameters:**

-   `set` - Pointer to the set
-   `data` - The element to check

**Return Value:**

`true` if the element exists in the set, `false` otherwise. Returns `false` if `set` or `data` is `NULL`.

**Complexity:** O(1) average case

**Example:**

```c
Set set;
set_init(&set, NULL, NULL, NULL);

set_insert(&set, "element1");

if (set_contains(&set, "element1")) {
    printf("Element exists\n");
}

if (!set_contains(&set, "nonexistent")) {
    printf("Element does not exist\n");
}
```

---

### set_intersection

```c
int set_intersection(Set* result, const Set* set1, const Set* set2);
```

Computes the intersection of two sets.

**Parameters:**

-   `result` - Pointer to the result set
-   `set1` - Pointer to the first set
-   `set2` - Pointer to the second set

**Return Value:**

Returns `0` on success, `-1` on failure.

**Description:**

Computes the intersection of `set1` and `set2`, storing the result in `result`. The result set is cleared before the operation. The result set inherits the hash and match functions from `set1`. The result set keeps shadow copies of the elements (does not take ownership), so the original sets remain valid and their elements are not destroyed when the result set is destroyed.

**Complexity:** O(n) where n is the size of the smaller set

**Example:**

```c
Set set1, set2, result;
set_init(&set1, NULL, NULL, NULL);
set_init(&set2, NULL, NULL, NULL);
set_init(&result, NULL, NULL, NULL);

set_insert(&set1, "a");
set_insert(&set1, "b");
set_insert(&set2, "b");
set_insert(&set2, "c");

set_intersection(&result, &set1, &set2);
// result contains {"b"}

set_destroy(&set1);
set_destroy(&set2);
set_destroy(&result);
```

---

### set_union

```c
int set_union(Set* result, const Set* set1, const Set* set2);
```

Computes the union of two sets.

**Parameters:**

-   `result` - Pointer to the result set
-   `set1` - Pointer to the first set
-   `set2` - Pointer to the second set

**Return Value:**

Returns `0` on success, `-1` on failure.

**Description:**

Computes the union of `set1` and `set2`, storing the result in `result`. The result set is cleared before the operation. The result set inherits the hash and match functions from `set1`. The result set keeps shadow copies of the elements (does not take ownership), so the original sets remain valid and their elements are not destroyed when the result set is destroyed.

**Complexity:** O(n + m) where n and m are the sizes of the two sets

**Example:**

```c
Set set1, set2, result;
set_init(&set1, NULL, NULL, NULL);
set_init(&set2, NULL, NULL, NULL);
set_init(&result, NULL, NULL, NULL);

set_insert(&set1, "a");
set_insert(&set1, "b");
set_insert(&set2, "b");
set_insert(&set2, "c");

set_union(&result, &set1, &set2);
// result contains {"a", "b", "c"}

set_destroy(&set1);
set_destroy(&set2);
set_destroy(&result);
```

---

### set_difference

```c
int set_difference(Set* result, const Set* set1, const Set* set2);
```

Computes the difference of two sets.

**Parameters:**

-   `result` - Pointer to the result set
-   `set1` - Pointer to the first set
-   `set2` - Pointer to the second set

**Return Value:**

Returns `0` on success, `-1` on failure.

**Description:**

Computes the difference of `set1` and `set2` (elements in `set1` but not in `set2`), storing the result in `result`. The result set is cleared before the operation. The result set inherits the hash and match functions from `set1`. The result set keeps shadow copies of the elements (does not take ownership), so the original sets remain valid and their elements are not destroyed when the result set is destroyed.

**Complexity:** O(n) where n is the size of the first set

**Example:**

```c
Set set1, set2, result;
set_init(&set1, NULL, NULL, NULL);
set_init(&set2, NULL, NULL, NULL);
set_init(&result, NULL, NULL, NULL);

set_insert(&set1, "a");
set_insert(&set1, "b");
set_insert(&set2, "b");
set_insert(&set2, "c");

set_difference(&result, &set1, &set2);
// result contains {"a"}

set_destroy(&set1);
set_destroy(&set2);
set_destroy(&result);
```

---

### set_equal

```c
bool set_equal(const Set* set1, const Set* set2);
```

Checks if two sets are equal.

**Parameters:**

-   `set1` - Pointer to the first set
-   `set2` - Pointer to the second set

**Return Value:**

`true` if the two sets are equal (contain the same elements), `false` otherwise. Returns `false` if either set is `NULL`.

**Complexity:** O(n) where n is the size of the sets

**Example:**

```c
Set set1, set2;
set_init(&set1, NULL, NULL, NULL);
set_init(&set2, NULL, NULL, NULL);

set_insert(&set1, "a");
set_insert(&set1, "b");
set_insert(&set2, "a");
set_insert(&set2, "b");

if (set_equal(&set1, &set2)) {
    printf("Sets are equal\n");
}
```

---

### set_subset

```c
bool set_subset(const Set* set1, const Set* set2);
```

Checks if one set is a subset of another.

**Parameters:**

-   `set1` - Pointer to the first set
-   `set2` - Pointer to the second set

**Return Value:**

`true` if `set1` is a subset of `set2` (all elements of `set1` are in `set2`), `false` otherwise. Returns `false` if either set is `NULL`.

**Complexity:** O(n) where n is the size of the first set

**Example:**

```c
Set set1, set2;
set_init(&set1, NULL, NULL, NULL);
set_init(&set2, NULL, NULL, NULL);

set_insert(&set1, "a");
set_insert(&set1, "b");
set_insert(&set2, "a");
set_insert(&set2, "b");
set_insert(&set2, "c");

if (set_subset(&set1, &set2)) {
    printf("set1 is a subset of set2\n");
}
```

---

## Usage Examples

### Basic Usage

```c
#include <collection/set.h>
#include <stdio.h>

int main(void) {
    Set set;
    set_init(&set, NULL, NULL, NULL);

    // Insert elements
    set_insert(&set, "apple");
    set_insert(&set, "banana");
    set_insert(&set, "cherry");

    // Check membership
    if (set_contains(&set, "banana")) {
        printf("Banana is in the set\n");
    }

    // Get size
    printf("Set size: %zu\n", set_size(&set));

    // Remove element
    set_remove(&set, "banana");

    // Cleanup
    set_destroy(&set);
    return 0;
}
```

### With Custom Hash and Match Functions

```c
#include <collection/set.h>
#include <algorithm/hash.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    Set set;
    set_init(&set, hash_str,
        (bool (*)(const void *, const void *))strcmp, NULL);

    set_insert(&set, "key1");
    set_insert(&set, "key2");

    if (set_contains(&set, "key1")) {
        printf("Key1 exists\n");
    }

    set_destroy(&set);
    return 0;
}
```

### With Integer Elements

```c
#include <collection/set.h>
#include <algorithm/hash.h>
#include <stdio.h>

static uint32_t hash_int_ptr(const void *key) {
    return hash_int(*(int*)key);
}

static bool match_int_ptr(const void *key1, const void *key2) {
    return *(int*)key1 == *(int*)key2;
}

int main(void) {
    Set set;
    set_init(&set, hash_int_ptr, match_int_ptr, NULL);

    int val1 = 1, val2 = 2, val3 = 3;
    set_insert(&set, &val1);
    set_insert(&set, &val2);
    set_insert(&set, &val3);

    if (set_contains(&set, &val2)) {
        printf("Value 2 is in the set\n");
    }

    set_destroy(&set);
    return 0;
}
```

### Set Operations

```c
#include <collection/set.h>
#include <stdio.h>

int main(void) {
    Set set1, set2, result;
    set_init(&set1, NULL, NULL, NULL);
    set_init(&set2, NULL, NULL, NULL);
    set_init(&result, NULL, NULL, NULL);

    // Populate sets
    set_insert(&set1, "a");
    set_insert(&set1, "b");
    set_insert(&set1, "c");
    set_insert(&set2, "b");
    set_insert(&set2, "c");
    set_insert(&set2, "d");

    // Intersection
    set_intersection(&result, &set1, &set2);
    printf("Intersection size: %zu\n", set_size(&result));  // Prints 2

    // Union
    set_union(&result, &set1, &set2);
    printf("Union size: %zu\n", set_size(&result));  // Prints 4

    // Difference
    set_difference(&result, &set1, &set2);
    printf("Difference size: %zu\n", set_size(&result));  // Prints 1

    // Equality check
    if (set_equal(&set1, &set2)) {
        printf("Sets are equal\n");
    }

    // Subset check
    if (set_subset(&set1, &set2)) {
        printf("set1 is a subset of set2\n");
    }

    set_destroy(&set1);
    set_destroy(&set2);
    set_destroy(&result);
    return 0;
}
```

### With Custom Destructors

```c
#include <collection/set.h>
#include <stdlib.h>
#include <string.h>

void destroy_element(void *element) {
    free(element);
}

int main(void) {
    Set set;
    set_init(&set, NULL, NULL, destroy_element);

    char *elem1 = strdup("element1");
    char *elem2 = strdup("element2");
    char *elem3 = strdup("element3");

    set_insert(&set, elem1);
    set_insert(&set, elem2);
    set_insert(&set, elem3);

    // All elements will be freed automatically
    set_destroy(&set);
    return 0;
}
```

---

## Complexity

| Operation          | Complexity   |
| ------------------ | ------------ |
| `set_init`         | O(1)         |
| `set_destroy`      | O(n)         |
| `set_empty`        | O(1)         |
| `set_size`         | O(1)         |
| `set_insert`       | O(1) average |
| `set_remove`       | O(1) average |
| `set_clear`        | O(n)         |
| `set_contains`     | O(1) average |
| `set_intersection` | O(n)         |
| `set_union`        | O(n + m)     |
| `set_difference`   | O(n)         |
| `set_equal`        | O(n)         |
| `set_subset`       | O(n)         |

Where n and m are the number of elements. Average case complexities assume a good hash function that distributes keys uniformly.

---

## Important Notes

### Memory Management

-   The set manages its own memory allocation for internal structures
-   The destroy function is called automatically when elements are removed
-   The set never calls `free()` on element pointers unless the destroy function does so
-   Insert operations may fail due to memory allocation failure

### Hash Functions

-   A good hash function should distribute elements uniformly across buckets
-   The default hash function (`hash_str`) is suitable for string elements
-   For custom element types, provide an appropriate hash function
-   Poor hash functions can degrade performance to O(n) worst case

### Shadow Copies in Set Operations

-   When performing set operations (`set_intersection`, `set_union`, `set_difference`), the result set keeps shadow copies of elements
-   This means the result set does not take ownership of the elements
-   The original sets remain valid and their elements are not destroyed when the result set is destroyed
-   The result set's destroy function is set to `NULL` during these operations to prevent accidental destruction
-   The result set inherits the hash and match functions from `set1`, so both input sets should use compatible hash and match functions for correct behavior

### Duplicate Elements

-   Sets do not allow duplicate elements
-   Inserting an element that already exists is ignored and does not increase the set size
-   The set uses the match function to determine equality

### Thread Safety

Set operations are **not thread-safe**. External synchronization is required for concurrent access.

### Undefined Behavior

Avoid the following:

-   Passing `NULL` set pointers to any function (except where explicitly allowed)
-   Using a set after calling `set_destroy()` without re-initializing
-   Assuming insert operations always succeed (check return value)
-   Using elements after they have been destroyed by the set
-   Modifying elements in a way that affects their hash value or equality comparison while they are in the set
