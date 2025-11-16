---
title: Stack
description: Last-in-first-out (LIFO) data structure implementation
---

The `Stack` is a last-in-first-out (LIFO) data structure that provides efficient insertion and removal operations at one end.

## Header Files

To use the stack in your code, include the header file:

```c
#include <collection/stack.h>
```

This provides access to the `Stack` type and all stack functions.

## Macros

Efficient inline operations implemented as macros:

### stack_size

```c
stack_size(stack)
```

Returns the number of elements in the stack.

**Parameters:**

-   `stack` - Pointer to the stack

**Return Value:**

The number of elements currently stored in the stack.

**Complexity:** O(1)

---

### stack_empty

```c
stack_empty(stack)
```

Checks if the stack is empty.

**Parameters:**

-   `stack` - Pointer to the stack

**Return Value:**

`true` if the stack contains no elements (size == 0), `false` otherwise.

**Complexity:** O(1)

**Example:**

```c
Stack stack;
stack_init(&stack, NULL);

if (stack_empty(&stack)) {
    printf("Stack is empty\n");
}

stack_push(&stack, "element");

if (!stack_empty(&stack)) {
    printf("Stack has elements\n");
}
```

---

## Functions

Public interfaces for stack operations:

### stack_init

```c
void stack_init(Stack* stack, void (*destroy)(void *));
```

Initializes a stack to an empty state.

**Parameters:**

-   `stack` - Pointer to the stack to initialize
-   `destroy` - Optional destructor function called when elements are removed. Pass `NULL` if no cleanup is needed

**Complexity:** O(1)

**Example:**

```c
Stack stack;
stack_init(&stack, NULL);  // No destructor

Stack stack2;
stack_init(&stack2, free);  // Use free() as destructor
```

---

### stack_destroy

```c
void stack_destroy(Stack* stack);
```

Destroys the stack and frees all associated memory.

**Parameters:**

-   `stack` - Pointer to the stack to destroy

**Description:**

Calls the destroy function (if provided) on all elements, then frees all nodes. After destruction, the stack is reset to an empty state.

**Complexity:** O(n) where n is the number of elements

**Example:**

```c
Stack stack;
stack_init(&stack, NULL);
// ... use stack ...
stack_destroy(&stack);
```

---

### stack_push

```c
int stack_push(Stack* stack, void* data);
```

Pushes an element onto the top of the stack.

**Parameters:**

-   `stack` - Pointer to the stack
-   `data` - Pointer to the element to push (can be `NULL`)

**Return Value:**

Returns 0 on success, -1 on failure.

**Description:**

Pushes a new element onto the top of the stack. The element becomes the new top element.

**Complexity:** O(1)

**Example:**

```c
if (stack_push(&stack, "first") == 0) {
    // Successfully pushed
}
stack_push(&stack, "second");
stack_push(&stack, "third");
// Stack now contains (from top to bottom): ["third", "second", "first"]
```

---

### stack_pop

```c
int stack_pop(Stack* stack, void** data);
```

Removes the top element from the stack.

**Parameters:**

-   `stack` - Pointer to the stack
-   `data` - Optional pointer to store the popped element. Pass `NULL` if the element should be destroyed

**Return Value:**

Returns 0 on success, -1 on failure.

**Description:**

Removes the top element from the stack. If `data` is not `NULL`, the removed element is stored at `*data` and the destroy function is **not** called (user takes ownership). If `data` is `NULL`, the destroy function is called on the element if provided.

**Complexity:** O(1)

**Example:**

```c
// Without retrieving data (destroy function is called)
stack_push(&stack, element);
stack_pop(&stack, NULL);

// With retrieving data (user responsibility to free)
void* element;
if (stack_pop(&stack, &element) == 0) {
    // Use element
    free(element);  // User must free if needed
}

// Pop from empty stack
if (stack_pop(&stack, NULL) == -1) {
    printf("Stack is empty\n");
}
```

**Note:**

-   When retrieving data, the caller is responsible for freeing the memory if necessary.

---

### stack_peek

```c
void* stack_peek(Stack* stack);
```

Returns the top element of the stack without removing it.

**Parameters:**

-   `stack` - Pointer to the stack

**Return Value:**

Pointer to the top element, or `NULL` if the stack is empty.

**Description:**

Returns a pointer to the top element without modifying the stack. The element remains on the stack.

**Complexity:** O(1)

**Example:**

```c
stack_push(&stack, "top");
stack_push(&stack, "middle");
stack_push(&stack, "bottom");

void* top = stack_peek(&stack);
printf("Top element: %s\n", (char*)top);  // Prints "bottom"
// Stack still contains all three elements
```

---

### stack_clear

```c
void stack_clear(Stack* stack);
```

Removes all elements from the stack.

**Parameters:**

-   `stack` - Pointer to the stack

**Description:**

Removes all elements from the stack and sets size to 0. The destroy function is called on each element if provided. After clearing, the stack is in an empty state and can be reused.

**Complexity:** O(n) where n is the number of elements

**Example:**

```c
stack_push(&stack, "one");
stack_push(&stack, "two");
stack_push(&stack, "three");
// size=3

stack_clear(&stack);
// size=0

// Stack can be reused
stack_push(&stack, "new");
// size=1
```

---

## Usage Examples

### Basic Usage

```c
#include <collection/stack.h>
#include <stdio.h>

int main(void) {
    Stack stack;
    stack_init(&stack, NULL);

    // Push elements
    stack_push(&stack, "Hello");
    stack_push(&stack, "World");
    stack_push(&stack, "!");

    // Peek at top
    printf("Top: %s\n", (char*)stack_peek(&stack));

    // Pop elements (LIFO order)
    while (!stack_empty(&stack)) {
        void* element;
        if (stack_pop(&stack, &element) == 0) {
            printf("%s\n", (char*)element);
        }
    }

    // Cleanup
    stack_destroy(&stack);
    return 0;
}
```

### With Custom Destructor

```c
#include <collection/stack.h>
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
    Stack stack;
    stack_init(&stack, destroy_person);

    // Create and push person
    Person* p = malloc(sizeof(Person));
    p->id = 1;
    p->name = strdup("Alice");
    stack_push(&stack, p);

    // All persons will be freed automatically
    stack_destroy(&stack);
    return 0;
}
```

### LIFO Order Verification

```c
#include <collection/stack.h>
#include <stdio.h>

int main(void) {
    Stack stack;
    stack_init(&stack, NULL);

    // Push elements
    for (int i = 1; i <= 5; i++) {
        stack_push(&stack, (void*)(long)i);
    }

    // Pop elements (should be in reverse order: 5, 4, 3, 2, 1)
    printf("Popping elements (LIFO order): ");
    while (!stack_empty(&stack)) {
        void* element;
        stack_pop(&stack, &element);
        printf("%d ", (int)(long)element);
    }
    printf("\n");

    stack_destroy(&stack);
    return 0;
}
```

### Expression Evaluation Pattern

```c
#include <collection/stack.h>
#include <stdio.h>

int main(void) {
    Stack stack;
    stack_init(&stack, NULL);

    // Simulate expression evaluation: push operands
    int a = 10, b = 20, c = 30;
    stack_push(&stack, (void*)(long)a);
    stack_push(&stack, (void*)(long)b);
    stack_push(&stack, (void*)(long)c);

    // Evaluate: c - b + a
    void* result;
    stack_pop(&stack, &result);  // c
    int val_c = (int)(long)result;

    stack_pop(&stack, &result);  // b
    int val_b = (int)(long)result;

    stack_pop(&stack, &result);  // a
    int val_a = (int)(long)result;

    int answer = val_c - val_b + val_a;
    printf("Result: %d\n", answer);  // 30 - 20 + 10 = 20

    stack_destroy(&stack);
    return 0;
}
```

---

## Complexity

| Operation       | Complexity |
| --------------- | ---------- |
| `stack_init`    | O(1)       |
| `stack_destroy` | O(n)       |
| `stack_size`    | O(1)       |
| `stack_empty`   | O(1)       |
| `stack_push`    | O(1)       |
| `stack_pop`     | O(1)       |
| `stack_peek`    | O(1)       |
| `stack_clear`   | O(n)       |

Where n is the number of elements.

---

## Important Notes

### Memory Management

-   The stack manages its own memory allocation for nodes
-   The destroy function is called automatically when elements are removed
-   The stack never calls `free()` on element data pointers unless the destroy function does so
-   Push operations may fail due to memory allocation failure

### LIFO Order

-   The stack follows Last-In-First-Out (LIFO) ordering
-   The most recently pushed element is at the top
-   `stack_peek()` returns the top element without removing it
-   `stack_pop()` removes and returns the top element

### Thread Safety

Stack operations are **not thread-safe**. External synchronization is required for concurrent access.

### Undefined Behavior

Avoid the following:

-   Passing `NULL` stack pointers to any function
-   Using a stack after calling `stack_destroy()` without re-initializing
-   Assuming push operations always succeed (check return value)
-   Using data retrieved from `stack_pop()` after the stack has been destroyed or cleared
