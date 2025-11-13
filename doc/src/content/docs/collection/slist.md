---
title: SList
description: Singly linked list implementation with efficient front/back operations
---

The `SList` is a singly linked list data structure that provides efficient insertion and deletion at both ends.

## Header Files

To use the singly linked list in your code, include the header file:

```c
#include <collection/slist.h>
```

This provides access to the `SList` and `SListNode` types and all list functions.

## Macros

Efficient inline operations implemented as macros:

### slist_size

```c
slist_size(slist)
```

Returns the number of elements in the list.

**Parameters:**

-   `slist` - Pointer to the list

**Return Value:**

The number of elements currently stored in the list.

**Complexity:** O(1)

---

### slist_empty

```c
slist_empty(slist)
```

Checks if the list is empty.

**Parameters:**

-   `slist` - Pointer to the list

**Return Value:**

`true` if the list contains no elements (size == 0), `false` otherwise.

**Complexity:** O(1)

**Example:**

```c
SList list;
slist_init(&list, NULL);

if (slist_empty(&list)) {
    printf("List is empty\n");
}

slist_push_front(&list, "element");

if (!slist_empty(&list)) {
    printf("List has elements\n");
}
```

---

### slist_front

```c
slist_front(slist)
```

Returns the first node in the list.

**Parameters:**

-   `slist` - Pointer to the list

**Return Value:**

Pointer to the first node, or `NULL` if the list is empty.

**Complexity:** O(1)

**Example:**

```c
SListNode* node = slist_front(&list);
if (node != NULL) {
    printf("First element: %s\n", (char*)node->data);
}
```

---

### slist_back

```c
slist_back(slist)
```

Returns the last node in the list.

**Parameters:**

-   `slist` - Pointer to the list

**Return Value:**

Pointer to the last node, or `NULL` if the list is empty.

**Complexity:** O(1)

**Example:**

```c
SListNode* node = slist_back(&list);
if (node != NULL) {
    printf("Last element: %s\n", (char*)node->data);
}
```

---

### slist_next

```c
slist_next(node)
```

Gets the next node in the list.

**Parameters:**

-   `node` - Pointer to the current node

**Return Value:**

Pointer to the next node, or `NULL` if this is the last node.

**Complexity:** O(1)

**Example:**

```c
// Iterate through the list
for (SListNode* node = slist_front(&list); node != NULL; node = slist_next(node)) {
    printf("%s\n", (char*)node->data);
}
```

---

### slist_data

```c
slist_data(node)
```

Gets the data stored in a node.

**Parameters:**

-   `node` - Pointer to the node

**Return Value:**

Pointer to the data stored in the node.

**Complexity:** O(1)

**Example:**

```c
SListNode* node = slist_front(&list);
if (node != NULL) {
    void* data = slist_data(node);
    printf("Data: %s\n", (char*)data);
}
```

---

### slist_head

```c
slist_head(slist)
```

Returns the head (first) node of the list.

**Parameters:**

-   `slist` - Pointer to the list

**Return Value:**

Pointer to the head node, or `NULL` if the list is empty.

**Complexity:** O(1)

**Note:** This macro is equivalent to `slist_front()`.

**Example:**

```c
SListNode* head = slist_head(&list);
if (head != NULL) {
    printf("Head data: %s\n", (char*)head->data);
}
```

---

### slist_tail

```c
slist_tail(slist)
```

Returns the tail (last) node of the list.

**Parameters:**

-   `slist` - Pointer to the list

**Return Value:**

Pointer to the tail node, or `NULL` if the list is empty.

**Complexity:** O(1)

**Note:** This macro is equivalent to `slist_back()`.

**Example:**

```c
SListNode* tail = slist_tail(&list);
if (tail != NULL) {
    printf("Tail data: %s\n", (char*)tail->data);
}
```

---

## Functions

Public interfaces for singly linked list operations:

### slist_init

```c
void slist_init(SList* slist, void (*destroy)(void *));
```

Initializes a singly linked list to an empty state.

**Parameters:**

-   `slist` - Pointer to the list to initialize
-   `destroy` - Optional destructor function called when elements are removed. Pass `NULL` if no cleanup is needed

**Complexity:** O(1)

**Example:**

```c
SList list;
slist_init(&list, NULL);  // No destructor

SList list2;
slist_init(&list2, free);  // Use free() as destructor
```

---

### slist_destroy

```c
void slist_destroy(SList* slist);
```

Destroys the singly linked list and frees all associated memory.

**Parameters:**

-   `slist` - Pointer to the list to destroy

**Description:**

Calls the destroy function (if provided) on all elements, then frees all nodes. After destruction, the list is reset to an empty state.

**Complexity:** O(n) where n is the number of elements

**Example:**

```c
SList list;
slist_init(&list, NULL);
// ... use list ...
slist_destroy(&list);
```

---

### slist_push_front

```c
void slist_push_front(SList* slist, void* data);
```

Inserts an element at the beginning of the list.

**Parameters:**

-   `slist` - Pointer to the list
-   `data` - Pointer to the element to insert (can be `NULL`)

**Description:**

Inserts a new node with the given data at the front of the list. This operation does not invalidate any node pointers.

**Complexity:** O(1)

**Example:**

```c
slist_push_front(&list, "first");
slist_push_front(&list, "second");
// List now contains: ["second", "first"]
```

---

### slist_push_back

```c
void slist_push_back(SList* slist, void* data);
```

Appends an element to the end of the list.

**Parameters:**

-   `slist` - Pointer to the list
-   `data` - Pointer to the element to append (can be `NULL`)

**Description:**

Inserts a new node with the given data at the back of the list. This operation does not invalidate any node pointers.

**Complexity:** O(1)

**Example:**

```c
slist_push_back(&list, "first");
slist_push_back(&list, "second");
// List now contains: ["first", "second"]
```

---

### slist_pop_front

```c
int slist_pop_front(SList* slist, void** data);
```

Removes the first element from the list.

**Parameters:**

-   `slist` - Pointer to the list
-   `data` - Optional pointer to store the removed element. Pass `NULL` if the element should be destroyed

**Return Value:**

Returns `0` on success, `-1` if the list is empty.

**Description:**

Removes the first node from the list. If `data` is not `NULL`, the removed element is stored at `*data` and the destroy function is **not** called (user takes ownership). If `data` is `NULL`, the destroy function is called on the element if provided.

**Complexity:** O(1)

**Example:**

```c
// Without retrieving data (destroy function is called)
slist_push_front(&list, element);
slist_pop_front(&list, NULL);

// With retrieving data (user responsibility to free)
void* element;
if (slist_pop_front(&list, &element) == 0) {
    // Use element
    free(element);  // User must free if needed
}

// Pop from empty list
if (slist_pop_front(&list, NULL) == -1) {
    printf("List is empty\n");
}
```

**Note:**

-   When retrieving data, the caller is responsible for freeing the memory if necessary.

---

### slist_insert_after

```c
int slist_insert_after(SList* slist, SListNode* node, void* data);
```

Inserts an element after a given node.

**Parameters:**

-   `slist` - Pointer to the list
-   `node` - The node after which to insert
-   `data` - Pointer to the element to insert (can be `NULL`)

**Return Value:**

Returns `0` on success, `-1` on failure (NULL node).

**Description:**

Inserts a new node with the given data after the specified node. This operation does not invalidate any existing node pointers.

**Complexity:** O(1)

**Example:**

```c
SListNode* node = slist_front(&list);
if (slist_insert_after(&list, node, "new_element") == 0) {
    // Element inserted successfully
}
```

---

### slist_remove_after

```c
int slist_remove_after(SList* slist, SListNode* node, void** data);
```

Removes the element after a given node.

**Parameters:**

-   `slist` - Pointer to the list
-   `node` - The node whose next element will be removed
-   `data` - Optional pointer to store the removed element. Pass `NULL` if the element should be destroyed

**Return Value:**

Returns `0` on success, `-1` on failure (NULL node or no next node).

**Description:**

Removes the node after the specified node. If `data` is not `NULL`, the removed element is stored at `*data` and the destroy function is **not** called (user takes ownership). If `data` is `NULL`, the destroy function is called on the element if provided.

**Complexity:** O(1)

**Example:**

```c
// Without retrieving data (destroy function is called)
SListNode* node = slist_front(&list);
slist_remove_after(&list, node, NULL);

// With retrieving data (user responsibility to free)
void* removed_element;
if (slist_remove_after(&list, node, &removed_element) == 0) {
    // Use removed_element
    free(removed_element);  // User must free if needed
}
```

**Note:**

-   When retrieving data, the caller is responsible for freeing the memory if necessary.

---

## Usage Examples

### Basic Usage

```c
#include <collection/slist.h>
#include <stdio.h>

int main(void) {
    SList list;
    slist_init(&list, NULL);

    // Add elements
    slist_push_back(&list, "Hello");
    slist_push_back(&list, "World");
    slist_push_front(&list, "Start");

    // Iterate
    for (SListNode* node = slist_front(&list); node != NULL; node = slist_next(node)) {
        printf("%s\n", (char*)node->data);
    }

    // Cleanup
    slist_destroy(&list);
    return 0;
}
```

### With Custom Destructor

```c
#include <collection/slist.h>
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
    SList list;
    slist_init(&list, destroy_person);

    // Create and add person
    Person* p = malloc(sizeof(Person));
    p->id = 1;
    p->name = strdup("Alice");
    slist_push_back(&list, p);

    // All persons will be freed automatically
    slist_destroy(&list);
    return 0;
}
```

### Insert and Remove Operations

```c
#include <collection/slist.h>

int main(void) {
    SList list;
    slist_init(&list, NULL);

    slist_push_back(&list, "A");
    slist_push_back(&list, "C");

    // Insert "B" after "A"
    SListNode* node = slist_front(&list);
    slist_insert_after(&list, node, "B");
    // List: ["A", "B", "C"]

    // Remove "B"
    node = slist_front(&list);
    void* removed;
    slist_remove_after(&list, node, &removed);
    // List: ["A", "C"], removed = "B"

    slist_destroy(&list);
    return 0;
}
```

---

## Complexity

| Operation            | Complexity |
| -------------------- | ---------- |
| `slist_init`         | O(1)       |
| `slist_destroy`      | O(n)       |
| `slist_size`         | O(1)       |
| `slist_empty`        | O(1)       |
| `slist_front`        | O(1)       |
| `slist_back`         | O(1)       |
| `slist_push_front`   | O(1)       |
| `slist_push_back`    | O(1)       |
| `slist_pop_front`    | O(1)       |
| `slist_insert_after` | O(1)       |
| `slist_remove_after` | O(1)       |
| `slist_next`         | O(1)       |
| `slist_data`         | O(1)       |
| `slist_head`         | O(1)       |
| `slist_tail`         | O(1)       |

Where n is the number of elements.

---

## Important Notes

### Memory Management

-   The list manages its own memory allocation for nodes
-   The destroy function is called automatically when elements are removed
-   The list never calls `free()` on element data pointers unless the destroy function does so

### Node Pointers

-   Node pointers remain valid until the node is removed or the list is destroyed
-   Insertion operations do not invalidate any existing node pointers
-   After removing a node, any pointers to that node become invalid

### Thread Safety

List operations are **not thread-safe**. External synchronization is required for concurrent access.

### Undefined Behavior

Avoid the following:

-   Passing `NULL` list pointers to any function (except `slist_next`)
-   Using a list after calling `slist_destroy()` without re-initializing
-   Directly modifying node structure members
-   Using node pointers after the node has been removed
