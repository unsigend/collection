---
title: Queue
description: First-in-first-out (FIFO) data structure implementation
---

The `Queue` is a first-in-first-out (FIFO) data structure that provides efficient insertion at one end and removal from the other end.

## Header Files

To use the queue in your code, include the header file:

```c
#include <collection/queue.h>
```

This provides access to the `Queue` type and all queue functions.

## Functions

Public interfaces for queue operations:

### queue_init

```c
void queue_init(Queue* queue, void (*destroy)(void *));
```

Initializes a queue to an empty state.

**Parameters:**

-   `queue` - Pointer to the queue to initialize
-   `destroy` - Optional destructor function called when elements are removed. Pass `NULL` if no cleanup is needed

**Complexity:** O(1)

**Example:**

```c
Queue queue;
queue_init(&queue, NULL);  // No destructor

Queue queue2;
queue_init(&queue2, free);  // Use free() as destructor
```

---

### queue_empty

```c
bool queue_empty(const Queue* queue);
```

Checks if the queue is empty.

**Parameters:**

-   `queue` - Pointer to the queue

**Return Value:**

`true` if the queue contains no elements (size == 0), `false` otherwise. Returns `false` if `queue` is `NULL`.

**Complexity:** O(1)

**Example:**

```c
Queue queue;
queue_init(&queue, NULL);

if (queue_empty(&queue)) {
    printf("Queue is empty\n");
}

queue_enqueue(&queue, "element");

if (!queue_empty(&queue)) {
    printf("Queue has elements\n");
}
```

---

### queue_size

```c
size_t queue_size(const Queue* queue);
```

Returns the number of elements in the queue.

**Parameters:**

-   `queue` - Pointer to the queue

**Return Value:**

The number of elements currently stored in the queue. Returns `0` if `queue` is `NULL`.

**Complexity:** O(1)

**Example:**

```c
Queue queue;
queue_init(&queue, NULL);
queue_enqueue(&queue, "first");
queue_enqueue(&queue, "second");

size_t count = queue_size(&queue);
printf("Queue has %zu elements\n", count);
```

---

### queue_destroy

```c
void queue_destroy(Queue* queue);
```

Destroys the queue and frees all associated memory.

**Parameters:**

-   `queue` - Pointer to the queue to destroy

**Description:**

Calls the destroy function (if provided) on all elements, then frees all nodes. After destruction, the queue is reset to an empty state.

**Complexity:** O(n) where n is the number of elements

**Example:**

```c
Queue queue;
queue_init(&queue, NULL);
// ... use queue ...
queue_destroy(&queue);
```

---

### queue_enqueue

```c
int queue_enqueue(Queue* queue, void* data);
```

Adds an element to the back of the queue.

**Parameters:**

-   `queue` - Pointer to the queue
-   `data` - Pointer to the element to enqueue (can be `NULL`)

**Return Value:**

Returns 0 on success, -1 on failure.

**Description:**

Adds a new element to the back of the queue. The element will be dequeued after all previously enqueued elements.

**Complexity:** O(1)

**Example:**

```c
if (queue_enqueue(&queue, "first") == 0) {
    // Successfully enqueued
}
queue_enqueue(&queue, "second");
queue_enqueue(&queue, "third");
// Queue now contains (from front to back): ["first", "second", "third"]
```

---

### queue_dequeue

```c
int queue_dequeue(Queue* queue, void** data);
```

Removes the front element from the queue.

**Parameters:**

-   `queue` - Pointer to the queue
-   `data` - Optional pointer to store the dequeued element. Pass `NULL` if the element should be destroyed

**Return Value:**

Returns 0 on success, -1 on failure.

**Description:**

Removes the front element from the queue. If `data` is not `NULL`, the removed element is stored at `*data` and the destroy function is **not** called (user takes ownership). If `data` is `NULL`, the destroy function is called on the element if provided.

**Complexity:** O(1)

**Example:**

```c
// Without retrieving data (destroy function is called)
queue_enqueue(&queue, element);
queue_dequeue(&queue, NULL);

// With retrieving data (user responsibility to free)
void* element;
if (queue_dequeue(&queue, &element) == 0) {
    // Use element
    free(element);  // User must free if needed
}

// Dequeue from empty queue
if (queue_dequeue(&queue, NULL) == -1) {
    printf("Queue is empty\n");
}
```

**Note:**

-   When retrieving data, the caller is responsible for freeing the memory if necessary.

---

### queue_peek

```c
void* queue_peek(Queue* queue);
```

Returns the front element of the queue without removing it.

**Parameters:**

-   `queue` - Pointer to the queue

**Return Value:**

Pointer to the front element, or `NULL` if the queue is empty.

**Description:**

Returns a pointer to the front element without modifying the queue. The element remains in the queue.

**Complexity:** O(1)

**Example:**

```c
queue_enqueue(&queue, "first");
queue_enqueue(&queue, "second");
queue_enqueue(&queue, "third");

void* front = queue_peek(&queue);
printf("Front element: %s\n", (char*)front);  // Prints "first"
// Queue still contains all three elements
```

---

### queue_clear

```c
void queue_clear(Queue* queue);
```

Removes all elements from the queue.

**Parameters:**

-   `queue` - Pointer to the queue

**Description:**

Removes all elements from the queue and sets size to 0. The destroy function is called on each element if provided. After clearing, the queue is in an empty state and can be reused.

**Complexity:** O(n) where n is the number of elements

**Example:**

```c
queue_enqueue(&queue, "one");
queue_enqueue(&queue, "two");
queue_enqueue(&queue, "three");
// size=3

queue_clear(&queue);
// size=0

// Queue can be reused
queue_enqueue(&queue, "new");
// size=1
```

---

## Usage Examples

### Basic Usage

```c
#include <collection/queue.h>
#include <stdio.h>

int main(void) {
    Queue queue;
    queue_init(&queue, NULL);

    // Enqueue elements
    queue_enqueue(&queue, "Hello");
    queue_enqueue(&queue, "World");
    queue_enqueue(&queue, "!");

    // Peek at front
    printf("Front: %s\n", (char*)queue_peek(&queue));

    // Dequeue elements (FIFO order)
    while (!queue_empty(&queue)) {
        void* element;
        if (queue_dequeue(&queue, &element) == 0) {
            printf("%s\n", (char*)element);
        }
    }

    // Cleanup
    queue_destroy(&queue);
    return 0;
}
```

### With Custom Destructor

```c
#include <collection/queue.h>
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
    Queue queue;
    queue_init(&queue, destroy_person);

    // Create and enqueue person
    Person* p = malloc(sizeof(Person));
    p->id = 1;
    p->name = strdup("Alice");
    queue_enqueue(&queue, p);

    // All persons will be freed automatically
    queue_destroy(&queue);
    return 0;
}
```

### FIFO Order Verification

```c
#include <collection/queue.h>
#include <stdio.h>

int main(void) {
    Queue queue;
    queue_init(&queue, NULL);

    // Enqueue elements
    for (int i = 1; i <= 5; i++) {
        queue_enqueue(&queue, (void*)(long)i);
    }

    // Dequeue elements (should be in order: 1, 2, 3, 4, 5)
    printf("Dequeuing elements (FIFO order): ");
    while (!queue_empty(&queue)) {
        void* element;
        queue_dequeue(&queue, &element);
        printf("%d ", (int)(long)element);
    }
    printf("\n");

    queue_destroy(&queue);
    return 0;
}
```

### Task Queue Pattern

```c
#include <collection/queue.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    int priority;
    char* task;
} Task;

int main(void) {
    Queue queue;
    queue_init(&queue, NULL);

    // Enqueue tasks
    Task task1 = {1, "Process data"};
    Task task2 = {2, "Send email"};
    Task task3 = {3, "Generate report"};

    queue_enqueue(&queue, &task1);
    queue_enqueue(&queue, &task2);
    queue_enqueue(&queue, &task3);

    // Process tasks in order
    printf("Processing tasks:\n");
    while (!queue_empty(&queue)) {
        Task* task;
        queue_dequeue(&queue, (void**)&task);
        printf("Priority %d: %s\n", task->priority, task->task);
    }

    queue_destroy(&queue);
    return 0;
}
```

---

## Complexity

| Operation         | Complexity |
| ----------------- | ---------- |
| `queue_init`     | O(1)       |
| `queue_destroy`  | O(n)       |
| `queue_size`     | O(1)       |
| `queue_empty`    | O(1)       |
| `queue_enqueue`  | O(1)       |
| `queue_dequeue`  | O(1)       |
| `queue_peek`     | O(1)       |
| `queue_clear`    | O(n)       |

Where n is the number of elements.

---

## Important Notes

### Memory Management

-   The queue manages its own memory allocation for nodes
-   The destroy function is called automatically when elements are removed
-   The queue never calls `free()` on element data pointers unless the destroy function does so
-   Enqueue operations may fail due to memory allocation failure

### FIFO Order

-   The queue follows First-In-First-Out (FIFO) ordering
-   The first enqueued element is at the front
-   `queue_peek()` returns the front element without removing it
-   `queue_dequeue()` removes and returns the front element
-   New elements are added to the back of the queue

### Thread Safety

Queue operations are **not thread-safe**. External synchronization is required for concurrent access.

### Undefined Behavior

Avoid the following:

-   Passing `NULL` queue pointers to any function
-   Using a queue after calling `queue_destroy()` without re-initializing
-   Assuming enqueue operations always succeed (check return value)
-   Using data retrieved from `queue_dequeue()` after the queue has been destroyed or cleared

