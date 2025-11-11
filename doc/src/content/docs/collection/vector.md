---
title: Vector
description: Dynamic array implementation with automatic resizing
---

The `Vector` is a dynamic array data structure that automatically grows and shrinks as needed. It provides efficient random access to elements and is inspired by C++ `std::vector`.

## Overview

Vector is a sequence container that stores elements as void pointers with automatic memory management. The storage is handled automatically, being expanded and contracted as needed.

## Type Definition

```c
typedef struct {
    void **data;
    size_t size;
    size_t capacity;
    void (*destroy)(void *);
} Vector;
```

**Note:** Direct access to struct members is possible but not recommended. Use the provided interface functions instead.

---

## Functions

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

### vector_size

```c
size_t vector_size(Vector* vector);
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
size_t vector_capacity(Vector* vector);
```

Returns the number of elements that can be held in currently allocated storage.

**Parameters:**

-   `vector` - Pointer to the vector

**Return Value:**

The capacity of the currently allocated storage. This may be greater than or equal to the size.

**Complexity:** O(1)

**Note:** Use `vector_shrink_to_fit()` to reduce capacity to match size.

---

### vector_at

```c
void* vector_at(Vector* vector, size_t index);
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
void* vector_front(Vector* vector);
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
void* vector_back(Vector* vector);
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
void vector_resize(Vector* vector, size_t new_size);
```

Resizes the vector to contain `new_size` elements.

**Parameters:**

-   `vector` - Pointer to the vector
-   `new_size` - New size of the vector

**Behavior:**

-   If `new_size` is **greater** than current size: new elements are added and initialized to `NULL`
-   If `new_size` is **smaller** than current size: elements are removed and the destroy function is called on them
-   If `new_size` **equals** current size: no operation is performed

**Complexity:** O(n) where n is the larger of the old or new size

**Example:**

```c
vector_resize(&vec, 10);  // Create 10 NULL elements
vector_resize(&vec, 5);   // Remove last 5 elements
vector_resize(&vec, 0);   // Remove all elements
```

**Note:** When shrinking, capacity is not reduced. Use `vector_shrink_to_fit()` to free unused memory.

---

### vector_shrink_to_fit

```c
void vector_shrink_to_fit(Vector* vector);
```

Reduces capacity to match the current size, freeing unused memory.

**Parameters:**

-   `vector` - Pointer to the vector

**Complexity:** O(n) where n is the number of elements

**Example:**

```c
vector_resize(&vec, 100);     // size=100, capacity=100
vector_resize(&vec, 10);      // size=10, capacity=100
vector_shrink_to_fit(&vec);   // size=10, capacity=10
```

---

### vector_push_back

```c
void vector_push_back(Vector* vector, void* element);
```

Appends an element to the end of the vector.

**Parameters:**

-   `vector` - Pointer to the vector
-   `element` - Pointer to the element to append (can be `NULL`)

**Description:**

Appends the element to the end. If necessary, capacity is automatically increased. The vector takes ownership of the pointer but not the pointed-to object.

**Complexity:** O(1) amortized, O(n) worst case when reallocation occurs

**Example:**

```c
vector_push_back(&vec, "first");
vector_push_back(&vec, "second");
```

---

### vector_pop_back

```c
void vector_pop_back(Vector* vector);
```

Removes the last element from the vector.

**Parameters:**

-   `vector` - Pointer to the vector

**Description:**

Removes the last element and calls the destroy function on it if provided. Calling on an empty vector is safe and does nothing.

**Complexity:** O(1)

**Example:**

```c
vector_push_back(&vec, "element");
vector_pop_back(&vec);        // Removes "element"
vector_pop_back(&vec);        // Safe no-op on empty vector
```

**Note:** Capacity is not reduced. Use `vector_shrink_to_fit()` to free memory.

---

## Usage Examples

### Basic Usage

```c
#include <collection/vector.h>
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

---

## Complexity

| Operation              | Complexity     |
| ---------------------- | -------------- |
| `vector_init`          | O(1)           |
| `vector_destroy`       | O(n)           |
| `vector_size`          | O(1)           |
| `vector_capacity`      | O(1)           |
| `vector_at`            | O(1)           |
| `vector_front`         | O(1)           |
| `vector_back`          | O(1)           |
| `vector_resize`        | O(n)           |
| `vector_shrink_to_fit` | O(n)           |
| `vector_push_back`     | O(1) amortized |
| `vector_pop_back`      | O(1)           |

Where n is the number of elements.

---

## Important Notes

### Memory Management

-   The vector manages its own memory allocation
-   The destroy function is called automatically when elements are removed
-   The vector never calls `free()` on element pointers unless the destroy function does so

### Thread Safety

Vector operations are **not thread-safe**. External synchronization is required for concurrent access.

### Undefined Behavior

Avoid the following:

-   Passing `NULL` vector pointers to any function
-   Accessing elements at index >= size
-   Using a vector after calling `vector_destroy()` without re-initializing
-   Directly modifying `size` or `capacity` members
