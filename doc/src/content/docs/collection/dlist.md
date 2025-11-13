---
title: DList
description: Doubly linked list implementation with efficient bidirectional operations
---

The `DList` is a doubly linked list data structure that provides efficient insertion and deletion at both ends, as well as bidirectional traversal.

## Header Files

To use the doubly linked list in your code, include the header file:

```c
#include <collection/dlist.h>
```

This provides access to the `DList` and `DListNode` types and all list functions.

## Macros

Efficient inline operations implemented as macros:

### dlist_size

```c
dlist_size(dlist)
```

Returns the number of elements in the list.

**Parameters:**

-   `dlist` - Pointer to the list

**Return Value:**

The number of elements currently stored in the list.

**Complexity:** O(1)

---

### dlist_empty

```c
dlist_empty(dlist)
```

Checks if the list is empty.

**Parameters:**

-   `dlist` - Pointer to the list

**Return Value:**

`true` if the list contains no elements (size == 0), `false` otherwise.

**Complexity:** O(1)

**Example:**

```c
DList list;
dlist_init(&list, NULL);

if (dlist_empty(&list)) {
    printf("List is empty\n");
}

dlist_push_front(&list, "element");

if (!dlist_empty(&list)) {
    printf("List has elements\n");
}
```

---

### dlist_next

```c
dlist_next(node)
```

Gets the next node in the list.

**Parameters:**

-   `node` - Pointer to the current node

**Return Value:**

Pointer to the next node, or `NULL` if this is the last node.

**Complexity:** O(1)

**Example:**

```c
// Iterate forward through the list
for (DListNode* node = dlist_head(&list); node != NULL; node = dlist_next(node)) {
    printf("%s\n", (char*)node->data);
}
```

---

### dlist_prev

```c
dlist_prev(node)
```

Gets the previous node in the list.

**Parameters:**

-   `node` - Pointer to the current node

**Return Value:**

Pointer to the previous node, or `NULL` if this is the first node.

**Complexity:** O(1)

**Example:**

```c
// Iterate backward through the list
for (DListNode* node = dlist_tail(&list); node != NULL; node = dlist_prev(node)) {
    printf("%s\n", (char*)node->data);
}
```

---

### dlist_data

```c
dlist_data(node)
```

Gets the data stored in a node.

**Parameters:**

-   `node` - Pointer to the node

**Return Value:**

Pointer to the data stored in the node.

**Complexity:** O(1)

**Example:**

```c
DListNode* node = dlist_head(&list);
if (node != NULL) {
    void* data = dlist_data(node);
    printf("Data: %s\n", (char*)data);
}
```

---

### dlist_head

```c
dlist_head(dlist)
```

Returns the head (first) node of the list.

**Parameters:**

-   `dlist` - Pointer to the list

**Return Value:**

Pointer to the head node, or `NULL` if the list is empty.

**Complexity:** O(1)

**Example:**

```c
DListNode* head = dlist_head(&list);
if (head != NULL) {
    printf("Head data: %s\n", (char*)head->data);
}
```

---

### dlist_tail

```c
dlist_tail(dlist)
```

Returns the tail (last) node of the list.

**Parameters:**

-   `dlist` - Pointer to the list

**Return Value:**

Pointer to the tail node, or `NULL` if the list is empty.

**Complexity:** O(1)

**Example:**

```c
DListNode* tail = dlist_tail(&list);
if (tail != NULL) {
    printf("Tail data: %s\n", (char*)tail->data);
}
```

---

## Functions

Public interfaces for doubly linked list operations:

### dlist_init

```c
void dlist_init(DList* dlist, void (*destroy)(void *));
```

Initializes a doubly linked list to an empty state.

**Parameters:**

-   `dlist` - Pointer to the list to initialize
-   `destroy` - Optional destructor function called when elements are removed. Pass `NULL` if no cleanup is needed

**Complexity:** O(1)

**Example:**

```c
DList list;
dlist_init(&list, NULL);  // No destructor

DList list2;
dlist_init(&list2, free);  // Use free() as destructor
```

---

### dlist_destroy

```c
void dlist_destroy(DList* dlist);
```

Destroys the doubly linked list and frees all associated memory.

**Parameters:**

-   `dlist` - Pointer to the list to destroy

**Description:**

Calls the destroy function (if provided) on all elements, then frees all nodes. After destruction, the list is reset to an empty state.

**Complexity:** O(n) where n is the number of elements

**Example:**

```c
DList list;
dlist_init(&list, NULL);
// ... use list ...
dlist_destroy(&list);
```

---

### dlist_push_front

```c
int dlist_push_front(DList* dlist, void* data);
```

Inserts an element at the beginning of the list.

**Parameters:**

-   `dlist` - Pointer to the list
-   `data` - Pointer to the element to insert (can be `NULL`)

**Return Value:**

Returns `0` on success, `-1` on failure.

**Description:**

Inserts a new node with the given data at the front of the list. This operation does not invalidate any node pointers.

**Complexity:** O(1)

**Example:**

```c
if (dlist_push_front(&list, "first") == 0) {
    // Successfully inserted
}
dlist_push_front(&list, "second");
// List now contains: ["second", "first"]
```

---

### dlist_push_back

```c
int dlist_push_back(DList* dlist, void* data);
```

Appends an element to the end of the list.

**Parameters:**

-   `dlist` - Pointer to the list
-   `data` - Pointer to the element to append (can be `NULL`)

**Return Value:**

Returns `0` on success, `-1` on failure.

**Description:**

Inserts a new node with the given data at the back of the list. This operation does not invalidate any node pointers.

**Complexity:** O(1)

**Example:**

```c
dlist_push_back(&list, "first");
dlist_push_back(&list, "second");
// List now contains: ["first", "second"]
```

---

### dlist_pop_front

```c
int dlist_pop_front(DList* dlist, void** data);
```

Removes the first element from the list.

**Parameters:**

-   `dlist` - Pointer to the list
-   `data` - Optional pointer to store the removed element. Pass `NULL` if the element should be destroyed

**Return Value:**

Returns `0` on success, `-1` on failure.

**Description:**

Removes the first node from the list. If `data` is not `NULL`, the removed element is stored at `*data` and the destroy function is **not** called (user takes ownership). If `data` is `NULL`, the destroy function is called on the element if provided.

**Complexity:** O(1)

**Example:**

```c
// Without retrieving data (destroy function is called)
dlist_push_front(&list, element);
dlist_pop_front(&list, NULL);

// With retrieving data (user responsibility to free)
void* element;
if (dlist_pop_front(&list, &element) == 0) {
    // Use element
    free(element);  // User must free if needed
}

// Pop from empty list
if (dlist_pop_front(&list, NULL) == -1) {
    printf("List is empty\n");
}
```

**Note:**

-   When retrieving data, the caller is responsible for freeing the memory if necessary.

---

### dlist_pop_back

```c
int dlist_pop_back(DList* dlist, void** data);
```

Removes the last element from the list.

**Parameters:**

-   `dlist` - Pointer to the list
-   `data` - Optional pointer to store the removed element. Pass `NULL` if the element should be destroyed

**Return Value:**

Returns `0` on success, `-1` on failure.

**Description:**

Removes the last node from the list. If `data` is not `NULL`, the removed element is stored at `*data` and the destroy function is **not** called (user takes ownership). If `data` is `NULL`, the destroy function is called on the element if provided.

**Complexity:** O(1)

**Example:**

```c
// Without retrieving data (destroy function is called)
dlist_push_back(&list, element);
dlist_pop_back(&list, NULL);

// With retrieving data (user responsibility to free)
void* element;
if (dlist_pop_back(&list, &element) == 0) {
    // Use element
    free(element);  // User must free if needed
}

// Pop from empty list
if (dlist_pop_back(&list, NULL) == -1) {
    printf("List is empty\n");
}
```

**Note:**

-   When retrieving data, the caller is responsible for freeing the memory if necessary.
-   This operation is O(1) for doubly linked lists, unlike O(n) for singly linked lists.

---

### dlist_insert_after

```c
int dlist_insert_after(DList* dlist, DListNode* node, void* data);
```

Inserts an element after a given node.

**Parameters:**

-   `dlist` - Pointer to the list
-   `node` - The node after which to insert
-   `data` - Pointer to the element to insert (can be `NULL`)

**Return Value:**

Returns `0` on success, `-1` on failure.

**Description:**

Inserts a new node with the given data after the specified node. This operation does not invalidate any existing node pointers.

**Complexity:** O(1)

**Example:**

```c
DListNode* node = dlist_head(&list);
if (dlist_insert_after(&list, node, "new_element") == 0) {
    // Element inserted successfully
}
```

---

### dlist_insert_before

```c
int dlist_insert_before(DList* dlist, DListNode* node, void* data);
```

Inserts an element before a given node.

**Parameters:**

-   `dlist` - Pointer to the list
-   `node` - The node before which to insert
-   `data` - Pointer to the element to insert (can be `NULL`)

**Return Value:**

Returns `0` on success, `-1` on failure.

**Description:**

Inserts a new node with the given data before the specified node. This operation does not invalidate any existing node pointers.

**Complexity:** O(1)

**Example:**

```c
DListNode* node = dlist_tail(&list);
if (dlist_insert_before(&list, node, "new_element") == 0) {
    // Element inserted successfully
}
```

---

### dlist_remove

```c
int dlist_remove(DList* dlist, DListNode* node, void** data);
```

Removes a specific node from the list.

**Parameters:**

-   `dlist` - Pointer to the list
-   `node` - The node to remove
-   `data` - Optional pointer to store the removed element. Pass `NULL` if the element should be destroyed

**Return Value:**

Returns `0` on success, `-1` on failure.

**Description:**

Removes the specified node from the list. If `data` is not `NULL`, the removed element is stored at `*data` and the destroy function is **not** called (user takes ownership). If `data` is `NULL`, the destroy function is called on the element if provided.

**Complexity:** O(1)

**Example:**

```c
// Without retrieving data (destroy function is called)
DListNode* node = dlist_head(&list);
dlist_remove(&list, node, NULL);

// With retrieving data (user responsibility to free)
void* removed_element;
if (dlist_remove(&list, node, &removed_element) == 0) {
    // Use removed_element
    free(removed_element);  // User must free if needed
}
```

**Note:**

-   When retrieving data, the caller is responsible for freeing the memory if necessary.
-   After removing a node, the node pointer becomes invalid and should not be used.

---

### dlist_clear

```c
void dlist_clear(DList* dlist);
```

Removes all elements from the list.

**Parameters:**

-   `dlist` - Pointer to the list

**Description:**

Removes all elements from the list and sets size to 0. The destroy function is called on each element if provided. After clearing, the list is in an empty state and can be reused.

**Complexity:** O(n) where n is the number of elements

**Example:**

```c
dlist_push_back(&list, "one");
dlist_push_back(&list, "two");
dlist_push_back(&list, "three");
// size=3

dlist_clear(&list);
// size=0, head=NULL, tail=NULL

// List can be reused
dlist_push_back(&list, "new");
// size=1
```

---

## Usage Examples

### Basic Usage

```c
#include <collection/dlist.h>
#include <stdio.h>

int main(void) {
    DList list;
    dlist_init(&list, NULL);

    // Add elements
    dlist_push_back(&list, "Hello");
    dlist_push_back(&list, "World");
    dlist_push_front(&list, "Start");

    // Iterate forward
    for (DListNode* node = dlist_head(&list); node != NULL; node = dlist_next(node)) {
        printf("%s\n", (char*)node->data);
    }

    // Iterate backward
    for (DListNode* node = dlist_tail(&list); node != NULL; node = dlist_prev(node)) {
        printf("%s\n", (char*)node->data);
    }

    // Cleanup
    dlist_destroy(&list);
    return 0;
}
```

### With Custom Destructor

```c
#include <collection/dlist.h>
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
    DList list;
    dlist_init(&list, destroy_person);

    // Create and add person
    Person* p = malloc(sizeof(Person));
    p->id = 1;
    p->name = strdup("Alice");
    dlist_push_back(&list, p);

    // All persons will be freed automatically
    dlist_destroy(&list);
    return 0;
}
```

### Insert and Remove Operations

```c
#include <collection/dlist.h>

int main(void) {
    DList list;
    dlist_init(&list, NULL);

    dlist_push_back(&list, "A");
    dlist_push_back(&list, "C");

    // Insert "B" after "A"
    DListNode* node = dlist_head(&list);
    dlist_insert_after(&list, node, "B");
    // List: ["A", "B", "C"]

    // Insert "D" before "C"
    node = dlist_tail(&list);
    dlist_insert_before(&list, node, "D");
    // List: ["A", "B", "D", "C"]

    // Remove "D"
    node = dlist_prev(dlist_tail(&list));
    void* removed;
    dlist_remove(&list, node, &removed);
    // List: ["A", "B", "C"], removed = "D"

    dlist_destroy(&list);
    return 0;
}
```

### Bidirectional Traversal

```c
#include <collection/dlist.h>
#include <stdio.h>

int main(void) {
    DList list;
    dlist_init(&list, NULL);

    for (int i = 1; i <= 5; i++) {
        dlist_push_back(&list, (void*)(long)i);
    }

    // Forward traversal
    printf("Forward: ");
    for (DListNode* node = dlist_head(&list); node != NULL; node = dlist_next(node)) {
        printf("%d ", (int)(long)node->data);
    }
    printf("\n");

    // Backward traversal
    printf("Backward: ");
    for (DListNode* node = dlist_tail(&list); node != NULL; node = dlist_prev(node)) {
        printf("%d ", (int)(long)node->data);
    }
    printf("\n");

    dlist_destroy(&list);
    return 0;
}
```

---

## Complexity

| Operation             | Complexity |
| --------------------- | ---------- |
| `dlist_init`          | O(1)       |
| `dlist_destroy`       | O(n)       |
| `dlist_size`          | O(1)       |
| `dlist_empty`         | O(1)       |
| `dlist_push_front`    | O(1)       |
| `dlist_push_back`     | O(1)       |
| `dlist_pop_front`     | O(1)       |
| `dlist_pop_back`      | O(1)       |
| `dlist_clear`         | O(n)       |
| `dlist_insert_after`  | O(1)       |
| `dlist_insert_before` | O(1)       |
| `dlist_remove`        | O(1)       |
| `dlist_next`          | O(1)       |
| `dlist_prev`          | O(1)       |
| `dlist_data`          | O(1)       |
| `dlist_head`          | O(1)       |
| `dlist_tail`          | O(1)       |

Where n is the number of elements.

---

## Important Notes

### Memory Management

-   The list manages its own memory allocation for nodes
-   The destroy function is called automatically when elements are removed
-   The list never calls `free()` on element data pointers unless the destroy function does so
-   Push operations may fail due to memory allocation failure

### Node Pointers

-   Node pointers remain valid until the node is removed or the list is destroyed
-   Insertion operations do not invalidate any existing node pointers
-   After removing a node, any pointers to that node become invalid
-   Both forward and backward traversal are supported via `dlist_next()` and `dlist_prev()`

### Bidirectional Links

-   Each node maintains both `next` and `prev` pointers
-   The head node has `prev == NULL`
-   The tail node has `next == NULL`
-   All operations maintain the integrity of bidirectional links

### Thread Safety

List operations are **not thread-safe**. External synchronization is required for concurrent access.

### Undefined Behavior

Avoid the following:

-   Passing `NULL` list pointers to any function (except `dlist_next`, `dlist_prev`, `dlist_data`)
-   Using a list after calling `dlist_destroy()` without re-initializing
-   Directly modifying node structure members
-   Using node pointers after the node has been removed
-   Assuming push operations always succeed (check return value)

