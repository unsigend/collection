---
title: Vector
description: Dynamic array implementation with automatic resizing
---

The `Vector` is a dynamic array data structure that automatically grows and shrinks as needed. It provides efficient random access to elements and is inspired by C++ `std::vector`.

## Header Files

To use the vector in your code, include the header file:

```c
#include <collection/vector.h>
```

This provides access to the `Vector` type and all vector functions.

## Macros

Efficient inline operations implemented as macros:

### vector_size

```c
vector_size(vector)
```

Returns the number of elements in the vector.

**Parameters:**

-   `vector` - Pointer to the vector

**Return Value:**

The number of elements currently stored in the vector.

**Complexity:** O(1)

---

### vector_capacity

```c
vector_capacity(vector)
```

Returns the number of elements that can be held in currently allocated storage.

**Parameters:**

-   `vector` - Pointer to the vector

**Return Value:**

The capacity of the currently allocated storage. This may be greater than or equal to the size.

**Complexity:** O(1)

**Note:** Use `vector_shrink_to_fit()` to reduce capacity to match size.

---

### vector_empty

```c
vector_empty(vector)
```

Checks if the vector is empty.

**Parameters:**

-   `vector` - Pointer to the vector

**Return Value:**

`true` if the vector contains no elements (size == 0), `false` otherwise.

**Complexity:** O(1)

**Example:**

```c
Vector vec;
vector_init(&vec, NULL);

if (vector_empty(&vec)) {
    printf("Vector is empty\n");
}

vector_push_back(&vec, "element");

if (!vector_empty(&vec)) {
    printf("Vector has elements\n");
}
```

---

### vector_data

```c
vector_data(vector)
```

Returns a pointer to the underlying array serving as element storage.

**Parameters:**

-   `vector` - Pointer to the vector

**Return Value:**

Pointer to the underlying array, or `NULL` if the vector is empty.

**Complexity:** O(1)

**Description:**

Returns a direct pointer to the internal array of element pointers. This allows direct access to the storage for performance-critical operations. The returned pointer may be invalidated by any operation that changes the vector's capacity.

**Important:**

-   The pointer is invalidated after operations that cause reallocation (push_back, insert, resize with growth)
-   Direct modification of the array should be done carefully to avoid breaking vector invariants
-   Valid only for indices [0, size)

**Example:**

```c
Vector vec;
vector_init(&vec, NULL);

vector_push_back(&vec, "a");
vector_push_back(&vec, "b");
vector_push_back(&vec, "c");

void** data = vector_data(&vec);
for (size_t i = 0; i < vector_size(&vec); i++) {
    printf("%s\n", (char*)data[i]);
}
```

---

## Functions

Public interfaces for vector operations:

### vector_init

```c
void vector_init(Vector* vector, void (*destroy)(void *));
```

Initializes a vector to an empty state.

**Parameters:**

-   `vector` - Pointer to the vector to initialize
-   `destroy` - Optional destructor function called when elements are removed. Pass `NULL` if no cleanup is needed

**Complexity:** O(1)

**Example:**

```c
Vector vec;
vector_init(&vec, NULL);  // No destructor

Vector vec2;
vector_init(&vec2, free);  // Use free() as destructor
```

---

### vector_destroy

```c
void vector_destroy(Vector* vector);
```

Destroys the vector and frees all associated memory.

**Parameters:**

-   `vector` - Pointer to the vector to destroy

**Description:**

Calls the destroy function (if provided) on all elements, then frees the internal array. After destruction, the vector is reset to an empty state.

**Complexity:** O(n) where n is the number of elements

**Example:**

```c
Vector vec;
vector_init(&vec, NULL);
// ... use vector ...
vector_destroy(&vec);
```

---

### vector_at

```c
void* vector_at(const Vector* vector, size_t index);
```

Returns the element at the specified index with bounds checking.

**Parameters:**

-   `vector` - Pointer to the vector
-   `index` - Position of the element to return (0-based)

**Return Value:**

Pointer to the element at the specified index.

**Complexity:** O(1)

**Important:** Accessing an index >= size causes program termination. Valid indices are [0, size).

**Example:**

```c
Vector vec;
vector_init(&vec, NULL);
vector_push_back(&vec, "hello");
char* str = (char*)vector_at(&vec, 0);  // Returns "hello"
```

---

### vector_front

```c
void* vector_front(const Vector* vector);
```

Returns the first element in the vector.

**Parameters:**

-   `vector` - Pointer to the vector

**Return Value:**

Pointer to the first element, or `NULL` if the vector is empty.

**Complexity:** O(1)

**Example:**

```c
void* first = vector_front(&vec);
if (first != NULL) {
    // Process first element
}
```

---

### vector_back

```c
void* vector_back(const Vector* vector);
```

Returns the last element in the vector.

**Parameters:**

-   `vector` - Pointer to the vector

**Return Value:**

Pointer to the last element, or `NULL` if the vector is empty.

**Complexity:** O(1)

---

### vector_resize

```c
int vector_resize(Vector* vector, size_t new_size);
```

Resizes the vector to contain `new_size` elements.

**Parameters:**

-   `vector` - Pointer to the vector
-   `new_size` - New size of the vector

**Return Value:**

Returns `0` (COLLECTION_SUCCESS) on success, `-1` (COLLECTION_FAILURE) on failure.

**Behavior:**

-   If `new_size` is **greater** than current size: new elements are added and initialized to `NULL`
-   If `new_size` is **smaller** than current size: elements are removed and the destroy function is called on them
-   If `new_size` **equals** current size: no operation is performed
-   If memory allocation fails when growing, the vector remains unchanged and `-1` is returned

**Complexity:** O(n) where n is the larger of the old or new size

**Example:**

```c
if (vector_resize(&vec, 10) == 0) {  // Create 10 NULL elements
    // Success
}

if (vector_resize(&vec, 5) != 0) {   // Remove last 5 elements
    // Handle allocation failure (though shrinking typically succeeds)
    fprintf(stderr, "Resize failed\n");
}

vector_resize(&vec, 0);   // Remove all elements (always succeeds)
```

**Note:** When shrinking, capacity is not reduced. Use `vector_shrink_to_fit()` to free unused memory.

---

### vector_shrink_to_fit

```c
int vector_shrink_to_fit(Vector* vector);
```

Reduces capacity to match the current size, freeing unused memory.

**Parameters:**

-   `vector` - Pointer to the vector

**Return Value:**

Returns `0` (COLLECTION_SUCCESS) on success, `-1` (COLLECTION_FAILURE) on failure.

**Description:**

Attempts to reduce the allocated memory to match the current size. If memory allocation fails during the shrinking operation, the vector remains unchanged with its original capacity and `-1` is returned. The vector contents are never lost.

**Complexity:** O(n) where n is the number of elements

**Example:**

```c
vector_resize(&vec, 100);     // size=100, capacity=100
vector_resize(&vec, 10);      // size=10, capacity=100

if (vector_shrink_to_fit(&vec) == 0) {   // size=10, capacity=10
    // Successfully freed unused memory
} else {
    // Shrink failed but vector is still valid with old capacity
    fprintf(stderr, "Failed to shrink, but vector is still usable\n");
}
```

---

### vector_push_back

```c
int vector_push_back(Vector* vector, void* element);
```

Appends an element to the end of the vector.

**Parameters:**

-   `vector` - Pointer to the vector
-   `element` - Pointer to the element to append (can be `NULL`)

**Return Value:**

Returns `0` (COLLECTION_SUCCESS) on success, `-1` (COLLECTION_FAILURE) on failure.

**Description:**

Appends the element to the end. If necessary, capacity is automatically increased. The vector takes ownership of the pointer but not the pointed-to object. If memory allocation fails during reallocation, the vector remains unchanged and `-1` is returned.

**Complexity:** O(1) amortized, O(n) worst case when reallocation occurs

**Example:**

```c
if (vector_push_back(&vec, "first") == 0) {
    // Successfully added
}

if (vector_push_back(&vec, "second") != 0) {
    // Handle allocation failure
    fprintf(stderr, "Failed to push element\n");
}
```

---

### vector_pop_back

```c
int vector_pop_back(Vector* vector, void** data);
```

Removes the last element from the vector.

**Parameters:**

-   `vector` - Pointer to the vector
-   `data` - Optional pointer to store the removed element. Pass `NULL` if the element should be destroyed

**Return Value:**

Returns `0` (COLLECTION_SUCCESS) on success, `-1` (COLLECTION_FAILURE) on failure.

**Description:**

Removes the last element from the vector. If `data` is not `NULL`, the removed element is stored at `*data` and the destroy function is **not** called (user takes ownership). If `data` is `NULL`, the destroy function is called on the element if provided.

**Complexity:** O(1)

**Example:**

```c
#include <collection/vector.h>
#include <collection/common.h>
#include <stdlib.h>

// Without retrieving data (destroy function is called)
vector_push_back(&vec, element);
vector_pop_back(&vec, NULL);

// With retrieving data (user responsibility to free)
void* element;
if (vector_pop_back(&vec, &element) == COLLECTION_SUCCESS) {
    // Use element
    free(element);  // User must free if needed
}

// Pop from empty vector
if (vector_pop_back(&vec, NULL) == COLLECTION_FAILURE) {
    printf("Vector is empty\n");
}
```

**Note:**

-   Capacity is not reduced. Use `vector_shrink_to_fit()` to free memory.
-   When retrieving data, the caller is responsible for freeing the memory if necessary.

---

### vector_insert

```c
int vector_insert(Vector* vector, size_t index, void* element);
```

Inserts an element at the specified position.

**Parameters:**

-   `vector` - Pointer to the vector
-   `index` - Position where the element will be inserted (0-based)
-   `element` - Pointer to the element to insert (can be `NULL`)

**Return Value:**

Returns `0` (COLLECTION_SUCCESS) on success, `-1` (COLLECTION_FAILURE) on failure.

**Description:**

Inserts the element at the specified index, shifting all elements at and after that position to the right. The element can be inserted at any position from 0 to size (inclusive), where inserting at size is equivalent to `vector_push_back()`.

**Complexity:** O(n) where n is the number of elements after the insertion point

**Example:**

```c
vector_push_back(&vec, "first");
vector_push_back(&vec, "third");

if (vector_insert(&vec, 1, "second") == 0) {
    // Vector now contains: ["first", "second", "third"]
}

vector_insert(&vec, 0, "start");   // Insert at beginning
vector_insert(&vec, vector_size(&vec), "end");  // Insert at end
```

---

### vector_remove

```c
int vector_remove(Vector* vector, size_t index, void** data);
```

Removes the element at the specified position.

**Parameters:**

-   `vector` - Pointer to the vector
-   `index` - Position of the element to remove (0-based)
-   `data` - Optional pointer to store the removed element. Pass `NULL` if the element should be destroyed

**Return Value:**

Returns `0` (COLLECTION_SUCCESS) on success, `-1` (COLLECTION_FAILURE) on failure.

**Description:**

Removes the element at the specified index, shifting all elements after that position to the left. If `data` is not `NULL`, the removed element is stored at `*data` and the destroy function is **not** called (user takes ownership). If `data` is `NULL`, the destroy function is called on the element if provided.

**Complexity:** O(n) where n is the number of elements after the removal point

**Example:**

```c
// Without retrieving data (destroy function is called)
vector_remove(&vec, 1, NULL);

// With retrieving data (user responsibility to free)
void* removed_element;
if (vector_remove(&vec, 1, &removed_element) == 0) {
    // Use removed_element
    free(removed_element);  // User must free if needed
}

// Example with strings
// Vector contains: ["a", "b", "c", "d"]
vector_remove(&vec, 1, NULL);  // Remove "b", destroy is called
// Vector now contains: ["a", "c", "d"]

char* removed;
vector_remove(&vec, 0, (void**)&removed);  // Remove and retrieve "a"
// Vector now contains: ["c", "d"]
// User is responsible for freeing 'removed' if needed
```

**Note:**

-   Capacity is not reduced. Use `vector_shrink_to_fit()` to free memory.
-   When retrieving data, the caller is responsible for freeing the memory if necessary.

---

### vector_clear

```c
void vector_clear(Vector* vector);
```

Removes all elements from the vector.

**Parameters:**

-   `vector` - Pointer to the vector

**Description:**

Removes all elements from the vector and sets size to 0. The destroy function is called on each element if provided. The capacity remains unchanged.

**Complexity:** O(n) where n is the number of elements

**Example:**

```c
vector_push_back(&vec, "one");
vector_push_back(&vec, "two");
vector_push_back(&vec, "three");
// size=3, capacity>=3

vector_clear(&vec);
// size=0, capacity unchanged
```

**Note:** Capacity is not reduced. Use `vector_shrink_to_fit()` after clearing to free memory.

---

## Usage Examples

### Basic Usage

```c
#include <collection/vector.h>
#include <collection/common.h>
#include <stdio.h>

int main(void) {
    Vector vec;
    vector_init(&vec, NULL);

    // Add elements
    vector_push_back(&vec, "Hello");
    vector_push_back(&vec, "World");

    // Access elements
    printf("First: %s\n", (char*)vector_front(&vec));
    printf("Last: %s\n", (char*)vector_back(&vec));
    printf("Size: %zu\n", vector_size(&vec));

    // Iterate
    for (size_t i = 0; i < vector_size(&vec); i++) {
        printf("%s\n", (char*)vector_at(&vec, i));
    }

    // Cleanup
    vector_destroy(&vec);
    return 0;
}
```

### With Custom Destructor

```c
#include <collection/vector.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char* name;
} Person;

void destroy_person(void* data) {
    Person* p = (Person*)data;
    free(p->name);
    free(p);
}

int main(void) {
    Vector vec;
    vector_init(&vec, destroy_person);

    // Create and add person
    Person* p = malloc(sizeof(Person));
    p->id = 1;
    p->name = strdup("Alice");
    vector_push_back(&vec, p);

    // All persons will be freed automatically
    vector_destroy(&vec);
    return 0;
}
```

### Pre-allocation and Batch Operations

```c
#include <collection/vector.h>
#include <collection/common.h>

int main(void) {
    Vector vec;
    vector_init(&vec, NULL);

    // Pre-allocate space for 1000 elements
    vector_resize(&vec, 1000);

    // Fill with data
    for (size_t i = 0; i < 1000; i++) {
        vec.data[i] = (void*)(long)i;
    }

    // Process elements
    for (size_t i = 0; i < vector_size(&vec); i++) {
        long value = (long)vector_at(&vec, i);
        // ... process value ...
    }

    vector_destroy(&vec);
    return 0;
}
```

### Memory Leak Prevention with Destroy Functions

```c
#include <collection/vector.h>
#include <stdlib.h>
#include <string.h>

// Custom structure that needs cleanup
typedef struct {
    int id;
    char* name;
} Person;

// Destroy function to prevent memory leaks
void destroy_person(void* data) {
    if (data != NULL) {
        Person* p = (Person*)data;
        free(p->name);  // Free dynamically allocated string
        free(p);        // Free the structure itself
    }
}

int main(void) {
    Vector vec;
    vector_init(&vec, destroy_person);

    // Create and add persons
    for (int i = 0; i < 10; i++) {
        Person* p = malloc(sizeof(Person));
        p->id = i;
        p->name = strdup("Person Name");
        vector_push_back(&vec, p);
    }

    // Remove some elements - destroy function is called automatically
    vector_remove(&vec, 5, NULL);  // destroy_person called for removed element
    vector_pop_back(&vec, NULL);   // destroy_person called for popped element

    // Retrieve an element - destroy function is NOT called
    Person* retrieved = NULL;
    vector_pop_back(&vec, (void**)&retrieved);
    // Use retrieved person...
    // Must manually free if not destroyed by vector
    destroy_person(retrieved);

    // All remaining elements are automatically destroyed
    vector_destroy(&vec);  // destroy_person called for all remaining elements
    
    return 0;
}
```

---

## Complexity

| Operation              | Complexity     |
| ---------------------- | -------------- |
| `vector_init`          | O(1)           |
| `vector_destroy`       | O(n)           |
| `vector_size`          | O(1)           |
| `vector_capacity`      | O(1)           |
| `vector_empty`         | O(1)           |
| `vector_data`          | O(1)           |
| `vector_at`            | O(1)           |
| `vector_front`         | O(1)           |
| `vector_back`          | O(1)           |
| `vector_resize`        | O(n)           |
| `vector_shrink_to_fit` | O(n)           |
| `vector_push_back`     | O(1) amortized |
| `vector_pop_back`      | O(1)           |
| `vector_insert`        | O(n)           |
| `vector_remove`        | O(n)           |
| `vector_clear`         | O(n)           |

Where n is the number of elements.

---

## Important Notes

### Memory Management

-   The vector manages its own memory allocation for the internal array
-   The destroy function is called automatically when elements are removed through:
    -   `vector_destroy()` - destroys all elements
    -   `vector_clear()` - destroys all elements
    -   `vector_resize()` (when shrinking) - destroys removed elements
    -   `vector_pop_back()` (when `data` is `NULL`) - destroys the popped element
    -   `vector_remove()` (when `data` is `NULL`) - destroys the removed element
-   The destroy function is **not** called when:
    -   Elements are retrieved via `vector_pop_back()` or `vector_remove()` with a non-NULL `data` parameter
    -   Elements are accessed via `vector_at()`, `vector_front()`, or `vector_back()`
    -   The vector is resized to a larger size
    -   `vector_shrink_to_fit()` is called (only capacity changes, no elements removed)
-   The vector never calls `free()` on element pointers unless the destroy function does so
-   When elements are retrieved (not destroyed), the caller is responsible for freeing memory if needed

### Thread Safety

Vector operations are **not thread-safe**. External synchronization is required for concurrent access.

### Undefined Behavior

Avoid the following:

-   Passing `NULL` vector pointers to any function
-   Accessing elements at index >= size
-   Using a vector after calling `vector_destroy()` without re-initializing
-   Directly modifying `size` or `capacity` members
