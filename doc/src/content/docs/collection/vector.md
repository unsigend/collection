---
title: Vector
description: Dynamic array implementation with automatic resizing
---

The `Vector` is a dynamic array data structure that automatically grows and shrinks as needed. It provides efficient random access to elements and is inspired by C++ `std::vector`.

## Overview

A `Vector` is a generic container that stores elements of any type using void pointers. It maintains both a size (number of elements) and a capacity (allocated memory), automatically resizing when needed.

## Structure

```c
typedef struct {
    void **data;
    size_t size;
    size_t capacity;
    void (*destroy)(void *);
} Vector;
```

### Fields

- **`data`**: Pointer to the array of void pointers storing elements
- **`size`**: Current number of elements in the vector
- **`capacity`**: Total allocated capacity of the vector
- **`destroy`**: Optional function pointer to destroy elements when they are removed

## Initialization and Cleanup

### `vector_init`

Initialize a vector with an optional destroy function.

```c
void vector_init(Vector* vector, void (*destroy)(void *));
```

**Parameters:**
- `vector`: Pointer to the vector to initialize
- `destroy`: Function pointer to destroy elements (can be `NULL` if elements don't need cleanup)

**Example:**
```c
Vector vec;
vector_init(&vec, free);  // Use free() to destroy elements
```

### `vector_destroy`

Destroy a vector and free all associated memory.

```c
void vector_destroy(Vector* vector);
```

**Parameters:**
- `vector`: Pointer to the vector to destroy

**Example:**
```c
vector_destroy(&vec);
```

## Accessing Elements

### `vector_size`

Get the current number of elements in the vector.

```c
size_t vector_size(Vector* vector);
```

**Returns:** The number of elements in the vector

**Example:**
```c
size_t count = vector_size(&vec);
```

### `vector_capacity`

Get the current capacity of the vector.

```c
size_t vector_capacity(Vector* vector);
```

**Returns:** The allocated capacity of the vector

**Example:**
```c
size_t cap = vector_capacity(&vec);
```

### `vector_at`

Get the element at a specific index.

```c
void* vector_at(Vector* vector, size_t index);
```

**Parameters:**
- `vector`: Pointer to the vector
- `index`: Zero-based index of the element

**Returns:** Pointer to the element at the given index

**Note:** This function will exit the program if the index is out of bounds.

**Example:**
```c
int* value = (int*)vector_at(&vec, 0);
```

### `vector_front`

Get the first element of the vector.

```c
void* vector_front(Vector* vector);
```

**Returns:** Pointer to the first element, or `NULL` if the vector is empty

**Example:**
```c
int* first = (int*)vector_front(&vec);
```

### `vector_back`

Get the last element of the vector.

```c
void* vector_back(Vector* vector);
```

**Returns:** Pointer to the last element, or `NULL` if the vector is empty

**Example:**
```c
int* last = (int*)vector_back(&vec);
```

## Modifying Elements

### `vector_push_back`

Add an element to the end of the vector.

```c
void vector_push_back(Vector* vector, void* element);
```

**Parameters:**
- `vector`: Pointer to the vector
- `element`: Pointer to the element to add

**Note:** The vector will automatically resize if the capacity is insufficient. The resize factor is 2x.

**Example:**
```c
int* value = malloc(sizeof(int));
*value = 42;
vector_push_back(&vec, value);
```

### `vector_pop_back`

Remove the last element from the vector.

```c
void vector_pop_back(Vector* vector);
```

**Parameters:**
- `vector`: Pointer to the vector

**Note:** If a destroy function was provided during initialization, it will be called on the removed element. Does nothing if the vector is empty.

**Example:**
```c
vector_pop_back(&vec);
```

### `vector_resize`

Resize the vector to a new size.

```c
void vector_resize(Vector* vector, size_t new_size);
```

**Parameters:**
- `vector`: Pointer to the vector
- `new_size`: The new size for the vector

**Behavior:**
- If `new_size > size`: New elements are initialized to `NULL`
- If `new_size < size`: Elements beyond the new size are destroyed (if destroy function is provided)
- If `new_size > capacity`: The capacity is automatically increased

**Example:**
```c
vector_resize(&vec, 10);  // Resize to 10 elements
```

### `vector_shrink_to_fit`

Reduce the capacity to match the current size.

```c
void vector_shrink_to_fit(Vector* vector);
```

**Parameters:**
- `vector`: Pointer to the vector

**Example:**
```c
vector_shrink_to_fit(&vec);  // Free unused memory
```

## Complete Example

```c
#include <collection/vector.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    Vector vec;
    vector_init(&vec, free);
    
    // Add elements
    for (int i = 0; i < 5; i++) {
        int* value = malloc(sizeof(int));
        *value = i * 10;
        vector_push_back(&vec, value);
    }
    
    // Access elements
    printf("Size: %zu\n", vector_size(&vec));
    printf("Capacity: %zu\n", vector_capacity(&vec));
    
    // Iterate through elements
    for (size_t i = 0; i < vector_size(&vec); i++) {
        int* value = (int*)vector_at(&vec, i);
        printf("vec[%zu] = %d\n", i, *value);
    }
    
    // Get first and last
    int* first = (int*)vector_front(&vec);
    int* last = (int*)vector_back(&vec);
    printf("First: %d, Last: %d\n", *first, *last);
    
    // Remove last element
    vector_pop_back(&vec);
    printf("Size after pop: %zu\n", vector_size(&vec));
    
    // Cleanup
    vector_destroy(&vec);
    return 0;
}
```

## Performance Characteristics

- **Access:** O(1) for random access via `vector_at`
- **Insertion:** O(1) amortized for `vector_push_back` (due to exponential resizing)
- **Deletion:** O(1) for `vector_pop_back`
- **Resize:** O(n) where n is the number of elements

## Memory Management

The vector uses exponential growth (2x factor) to minimize reallocations. When elements are removed or the vector is resized to a smaller size, the destroy function (if provided) is called on each removed element to allow proper cleanup of dynamically allocated data.

