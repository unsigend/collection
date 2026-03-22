---
title: Stack
description: LIFO stack of fixed-size elements backed by a vector
---

A stack stores elements in last-in-first-out order. Each slot is exactly `elesz` bytes, the same as `vec_init`. Push adds on top, pop removes from top, peek reads the top without removing. Initialization, destructor behavior, and error conventions match the underlying vector.

## Header

```c
#include <stack.h>
```

## Struct

```c
struct stack {
  struct vector vec;
};
```

`vec` holds the stack state, including element size and the optional destructor given to `stack_init`.

## Macros

### stack_empty

```c
stack_empty(stack)
```

Returns non-zero if the stack contains no elements.

**Parameters**

- `stack` — pointer to the stack

---

### stack_size

```c
stack_size(stack)
```

Returns the current element count.

**Parameters**

- `stack` — pointer to the stack

---

## Functions

### stack_init

```c
int stack_init(struct stack *stack, size_t elesz, void (*destroy)(void *));
```

Initializes an empty stack with element size `elesz` and an optional destructor. Must be called before any other function. Returns 0 on success, -1 on error.

**Parameters**

- `stack` — pointer to an uninitialized stack struct
- `elesz` — byte size of each element, must be non-zero
- `destroy` — called on each element when it is discarded, or NULL for no-op

---

### stack_fini

```c
void stack_fini(struct stack *stack);
```

Destroys all elements and frees the buffer. No-op if `stack` is NULL. Does not free the `struct stack` itself.

**Parameters**

- `stack` — pointer to the stack

---

### stack_push

```c
int stack_push(struct stack *stack, void *ele);
```

Copies `elesz` bytes from `ele` onto the top, growing the buffer if needed. Returns 0 on success, -1 on error.

**Parameters**

- `stack` — pointer to the stack
- `ele` — pointer to the value to copy, must not be NULL

---

### stack_pop

```c
int stack_pop(struct stack *stack, void *dest);
```

Removes the top element. If `dest` is non-NULL, copies the element there and skips `destroy`. If `dest` is NULL and `destroy` is set, calls `destroy` on the element. Returns 0 on success, -1 if the stack is empty or `stack` is NULL.

**Parameters**

- `stack` — pointer to the stack
- `dest` — destination buffer of at least `elesz` bytes to receive the element, or NULL to invoke `destroy`

---

### stack_peek

```c
void *stack_peek(struct stack *stack);
```

Returns a pointer to the top element, or NULL if the stack is empty or `stack` is NULL.

**Parameters**

- `stack` — pointer to the stack

---

### stack_clear

```c
void stack_clear(struct stack *stack);
```

Removes all elements and resets size to zero, calling `destroy` on each when set. Does not free the buffer or the `struct stack` itself. No-op if `stack` is NULL.

**Parameters**

- `stack` — pointer to the stack

---

## Example

```c
#include <stack.h>

int main(void)
{
  struct stack s;
  int x, y;

  if (stack_init(&s, sizeof(int), NULL) != 0)
    return 1;

  x = 10;
  stack_push(&s, &x);
  x = 20;
  stack_push(&s, &x);

  if (!stack_empty(&s))
    y = *(int *)stack_peek(&s); /* y == 20 */

  stack_pop(&s, &y); /* y == 20 */
  stack_pop(&s, &y); /* y == 10 */

  stack_fini(&s);
  return 0;
}
```
