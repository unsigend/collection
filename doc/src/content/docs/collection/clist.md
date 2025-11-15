---
title: CList
description: Circular linked list implementation with efficient operations at both ends
---

The `CList` is a circular singly linked list data structure where the last node points back to the first node, enabling efficient insertion and deletion at both ends without a separate tail pointer.

## Header Files

To use the circular linked list in your code, include the header file:

```c
#include <collection/clist.h>
```

This provides access to the `CList` and `CListNode` types and all list functions.

## Macros

Efficient inline operations implemented as macros:

### clist_size

```c
clist_size(clist)
```

Returns the number of elements in the list.

**Parameters:**

-   `clist` - Pointer to the list

**Return Value:**

The number of elements currently stored in the list.

**Complexity:** O(1)

---

### clist_empty

```c
clist_empty(clist)
```

Checks if the list is empty.

**Parameters:**

-   `clist` - Pointer to the list

**Return Value:**

`true` if the list contains no elements (size == 0), `false` otherwise.

**Complexity:** O(1)

**Example:**

```c
CList list;
clist_init(&list, NULL);

if (clist_empty(&list)) {
    printf("List is empty\n");
}

clist_push_front(&list, "element");

if (!clist_empty(&list)) {
    printf("List has elements\n");
}
```

---

### clist_next

```c
clist_next(node)
```

Gets the next node in the list.

**Parameters:**

-   `node` - Pointer to the current node

**Return Value:**

Pointer to the next node. For the last node, returns the head (circular).

**Complexity:** O(1)

**Example:**

```c
// Traverse one complete circle
CListNode* start = clist_head(&list);
CListNode* node = start;
do {
    printf("%s\n", (char*)node->data);
    node = clist_next(node);
} while (node != start);
```

---

### clist_data

```c
clist_data(node)
```

Gets the data stored in a node.

**Parameters:**

-   `node` - Pointer to the node

**Return Value:**

Pointer to the data stored in the node.

**Complexity:** O(1)

**Example:**

```c
CListNode* node = clist_head(&list);
if (node != NULL) {
    void* data = clist_data(node);
    printf("Data: %s\n", (char*)data);
}
```

---

### clist_head

```c
clist_head(clist)
```

Returns the head (first) node of the list.

**Parameters:**

-   `clist` - Pointer to the list

**Return Value:**

Pointer to the head node, or `NULL` if the list is empty.

**Complexity:** O(1)

**Example:**

```c
CListNode* head = clist_head(&list);
if (head != NULL) {
    printf("Head data: %s\n", (char*)head->data);
}
```

---

### clist_tail

```c
clist_tail(clist)
```

Returns the tail (last) node of the list.

**Parameters:**

-   `clist` - Pointer to the list

**Return Value:**

Pointer to the tail node (the node before head in circular structure), or `NULL` if the list is empty.

**Complexity:** O(1)

**Note:** In a circular list, the tail is `head->prev` (the node whose `next` points to `head`).

**Example:**

```c
CListNode* tail = clist_tail(&list);
if (tail != NULL) {
    printf("Tail data: %s\n", (char*)tail->data);
}
```

---

## Functions

Public interfaces for circular linked list operations:

### clist_init

```c
void clist_init(CList* clist, void (*destroy)(void *));
```

Initializes a circular linked list to an empty state.

**Parameters:**

-   `clist` - Pointer to the list to initialize
-   `destroy` - Optional destructor function called when elements are removed. Pass `NULL` if no cleanup is needed

**Complexity:** O(1)

**Example:**

```c
CList list;
clist_init(&list, NULL);  // No destructor

CList list2;
clist_init(&list2, free);  // Use free() as destructor
```

---

### clist_destroy

```c
void clist_destroy(CList* clist);
```

Destroys the circular linked list and frees all associated memory.

**Parameters:**

-   `clist` - Pointer to the list to destroy

**Description:**

Calls the destroy function (if provided) on all elements, then frees all nodes. After destruction, the list is reset to an empty state.

**Complexity:** O(n) where n is the number of elements

**Example:**

```c
CList list;
clist_init(&list, NULL);
// ... use list ...
clist_destroy(&list);
```

---

### clist_push_front

```c
int clist_push_front(CList* clist, void* data);
```

Inserts an element at the beginning of the list.

**Parameters:**

-   `clist` - Pointer to the list
-   `data` - Pointer to the element to insert (can be `NULL`)

**Return Value:**

Returns `0` on success, `-1` on failure.

**Description:**

Inserts a new node with the given data at the front of the list. This operation does not invalidate any node pointers. The circular structure is maintained.

**Complexity:** O(1)

**Example:**

```c
if (clist_push_front(&list, "first") == 0) {
    // Successfully inserted
}
clist_push_front(&list, "second");
// List now contains: ["second", "first"]
```

---

### clist_push_back

```c
int clist_push_back(CList* clist, void* data);
```

Appends an element to the end of the list.

**Parameters:**

-   `clist` - Pointer to the list
-   `data` - Pointer to the element to append (can be `NULL`)

**Return Value:**

Returns `0` on success, `-1` on failure.

**Description:**

Inserts a new node with the given data at the back of the list. This operation does not invalidate any node pointers. The circular structure is maintained.

**Complexity:** O(1)

**Example:**

```c
clist_push_back(&list, "first");
clist_push_back(&list, "second");
// List now contains: ["first", "second"]
```

---

### clist_pop_front

```c
int clist_pop_front(CList* clist, void** data);
```

Removes the first element from the list.

**Parameters:**

-   `clist` - Pointer to the list
-   `data` - Optional pointer to store the removed element. Pass `NULL` if the element should be destroyed

**Return Value:**

Returns `0` on success, `-1` on failure.

**Description:**

Removes the first node from the list. If `data` is not `NULL`, the removed element is stored at `*data` and the destroy function is **not** called (user takes ownership). If `data` is `NULL`, the destroy function is called on the element if provided.

**Complexity:** O(1)

**Example:**

```c
// Without retrieving data (destroy function is called)
clist_push_front(&list, element);
clist_pop_front(&list, NULL);

// With retrieving data (user responsibility to free)
void* element;
if (clist_pop_front(&list, &element) == 0) {
    // Use element
    free(element);  // User must free if needed
}

// Pop from empty list
if (clist_pop_front(&list, NULL) == -1) {
    printf("List is empty\n");
}
```

**Note:**

-   When retrieving data, the caller is responsible for freeing the memory if necessary.

---

### clist_pop_back

```c
int clist_pop_back(CList* clist, void** data);
```

Removes the last element from the list.

**Parameters:**

-   `clist` - Pointer to the list
-   `data` - Optional pointer to store the removed element. Pass `NULL` if the element should be destroyed

**Return Value:**

Returns `0` on success, `-1` on failure.

**Description:**

Removes the last node from the list. If `data` is not `NULL`, the removed element is stored at `*data` and the destroy function is **not** called (user takes ownership). If `data` is `NULL`, the destroy function is called on the element if provided.

**Complexity:** O(1)

**Example:**

```c
// Without retrieving data (destroy function is called)
clist_push_back(&list, element);
clist_pop_back(&list, NULL);

// With retrieving data (user responsibility to free)
void* element;
if (clist_pop_back(&list, &element) == 0) {
    // Use element
    free(element);  // User must free if needed
}

// Pop from empty list
if (clist_pop_back(&list, NULL) == -1) {
    printf("List is empty\n");
}
```

**Note:**

-   When retrieving data, the caller is responsible for freeing the memory if necessary.
-   This operation is O(1) due to the circular structure.

---

### clist_insert_after

```c
int clist_insert_after(CList* clist, CListNode* node, void* data);
```

Inserts an element after a given node.

**Parameters:**

-   `clist` - Pointer to the list
-   `node` - The node after which to insert
-   `data` - Pointer to the element to insert (can be `NULL`)

**Return Value:**

Returns `0` on success, `-1` on failure.

**Description:**

Inserts a new node with the given data after the specified node. This operation does not invalidate any existing node pointers.

**Complexity:** O(1)

**Example:**

```c
CListNode* node = clist_head(&list);
if (clist_insert_after(&list, node, "new_element") == 0) {
    // Element inserted successfully
}
```

---

### clist_insert_before

```c
int clist_insert_before(CList* clist, CListNode* node, void* data);
```

Inserts an element before a given node.

**Parameters:**

-   `clist` - Pointer to the list
-   `node` - The node before which to insert
-   `data` - Pointer to the element to insert (can be `NULL`)

**Return Value:**

Returns `0` on success, `-1` on failure.

**Description:**

Inserts a new node with the given data before the specified node. This operation does not invalidate any existing node pointers. The circular structure is maintained.

**Complexity:** O(1)

**Example:**

```c
CListNode* node = clist_head(&list);
if (clist_insert_before(&list, node, "new_element") == 0) {
    // Element inserted successfully
}
```

---

### clist_remove

```c
int clist_remove(CList* clist, CListNode* node, void** data);
```

Removes a specific node from the list.

**Parameters:**

-   `clist` - Pointer to the list
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
CListNode* node = clist_head(&list);
clist_remove(&list, node, NULL);

// With retrieving data (user responsibility to free)
void* removed_element;
if (clist_remove(&list, node, &removed_element) == 0) {
    // Use removed_element
    free(removed_element);  // User must free if needed
}
```

**Note:**

-   When retrieving data, the caller is responsible for freeing the memory if necessary.
-   After removing a node, the node pointer becomes invalid and should not be used.

---

### clist_clear

```c
void clist_clear(CList* clist);
```

Removes all elements from the list.

**Parameters:**

-   `clist` - Pointer to the list

**Description:**

Removes all elements from the list and sets size to 0. The destroy function is called on each element if provided. After clearing, the list is in an empty state and can be reused.

**Complexity:** O(n) where n is the number of elements

**Example:**

```c
clist_push_back(&list, "one");
clist_push_back(&list, "two");
clist_push_back(&list, "three");
// size=3

clist_clear(&list);
// size=0, head=NULL

// List can be reused
clist_push_back(&list, "new");
// size=1
```

---

## Usage Examples

### Basic Usage

```c
#include <collection/clist.h>
#include <stdio.h>

int main(void) {
    CList list;
    clist_init(&list, NULL);

    // Add elements
    clist_push_back(&list, "Hello");
    clist_push_back(&list, "World");
    clist_push_front(&list, "Start");

    // Iterate through one complete circle
    CListNode* start = clist_head(&list);
    CListNode* node = start;
    do {
        printf("%s\n", (char*)node->data);
        node = clist_next(node);
    } while (node != start);

    // Cleanup
    clist_destroy(&list);
    return 0;
}
```

### With Custom Destructor

```c
#include <collection/clist.h>
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
    CList list;
    clist_init(&list, destroy_person);

    // Create and add person
    Person* p = malloc(sizeof(Person));
    p->id = 1;
    p->name = strdup("Alice");
    clist_push_back(&list, p);

    // All persons will be freed automatically
    clist_destroy(&list);
    return 0;
}
```

### Circular Traversal

```c
#include <collection/clist.h>
#include <stdio.h>

int main(void) {
    CList list;
    clist_init(&list, NULL);

    for (int i = 1; i <= 5; i++) {
        clist_push_back(&list, (void*)(long)i);
    }

    // Traverse multiple times around the circle
    CListNode* node = clist_head(&list);
    CListNode* start = node;
    
    printf("Three complete laps:\n");
    for (int lap = 0; lap < 3; lap++) {
        printf("Lap %d: ", lap + 1);
        for (int i = 0; i < 5; i++) {
            printf("%d ", (int)(long)node->data);
            node = clist_next(node);
        }
        printf("\n");
    }

    clist_destroy(&list);
    return 0;
}
```

### Insert and Remove Operations

```c
#include <collection/clist.h>

int main(void) {
    CList list;
    clist_init(&list, NULL);

    clist_push_back(&list, "A");
    clist_push_back(&list, "C");

    // Insert "B" after "A"
    CListNode* node = clist_head(&list);
    clist_insert_after(&list, node, "B");
    // List: ["A", "B", "C"]

    // Insert "Start" before "A"
    node = clist_head(&list);
    clist_insert_before(&list, node, "Start");
    // List: ["Start", "A", "B", "C"]

    // Remove "B"
    node = clist_head(&list);
    node = clist_next(node);  // "A"
    node = clist_next(node);  // "B"
    void* removed;
    clist_remove(&list, node, &removed);
    // List: ["Start", "A", "C"], removed = "B"

    clist_destroy(&list);
    return 0;
}
```

---

## Complexity

| Operation            | Complexity |
| -------------------- | ---------- |
| `clist_init`         | O(1)       |
| `clist_destroy`      | O(n)       |
| `clist_size`         | O(1)       |
| `clist_empty`        | O(1)       |
| `clist_push_front`   | O(1)       |
| `clist_push_back`    | O(1)       |
| `clist_pop_front`    | O(1)       |
| `clist_pop_back`     | O(1)       |
| `clist_clear`        | O(n)       |
| `clist_insert_after` | O(1)       |
| `clist_insert_before`| O(1)       |
| `clist_remove`       | O(1)       |
| `clist_next`         | O(1)       |
| `clist_data`         | O(1)       |
| `clist_head`         | O(1)       |
| `clist_tail`         | O(1)       |

Where n is the number of elements.

---

## Important Notes

### Circular Structure

-   The last node's `next` pointer always points back to the head
-   This enables O(1) operations at both ends without a separate tail pointer
-   Traversal naturally wraps around; use proper termination conditions to avoid infinite loops
-   For a single element, `node->next` points to itself

### Memory Management

-   The list manages its own memory allocation for nodes
-   The destroy function is called automatically when elements are removed
-   The list never calls `free()` on element data pointers unless the destroy function does so
-   Push operations may fail due to memory allocation failure

### Node Pointers

-   Node pointers remain valid until the node is removed or the list is destroyed
-   Insertion operations do not invalidate any existing node pointers
-   After removing a node, any pointers to that node become invalid
-   The circular structure is maintained automatically by all operations

### Thread Safety

List operations are **not thread-safe**. External synchronization is required for concurrent access.

### Undefined Behavior

Avoid the following:

-   Passing `NULL` list pointers to any function (except `clist_next`, `clist_data`)
-   Using a list after calling `clist_destroy()` without re-initializing
-   Directly modifying node structure members
-   Using node pointers after the node has been removed
-   Assuming push operations always succeed (check return value)
-   Infinite loops when traversing (always maintain a reference to the starting node)

