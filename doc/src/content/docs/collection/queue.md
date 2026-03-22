---
title: Queue
description: FIFO queue of fixed-size elements with enqueue, dequeue, and peek at the front
---

A queue stores elements in first-in-first-out order, each slot is exactly `elesz` bytes, enqueue adds at the back, dequeue removes from the front, peek reads the front without removing, initialization, destructor behavior, and error conventions follow the same pattern as `deq_init` and related deque APIs.

## Header

```c
#include <queue.h>
```

## Struct

```c
struct queue {
  struct deque deq;
};
```

`deq` holds the queue state, including element size and the optional destructor given to `queue_init`.

## Macros

### queue_empty

```c
queue_empty(queue)
```

Returns non-zero if the queue contains no elements.

**Parameters**

- `queue` — pointer to the queue

---

### queue_size

```c
queue_size(queue)
```

Returns the current element count.

**Parameters**

- `queue` — pointer to the queue

---

## Functions

### queue_init

```c
int queue_init(struct queue *queue, size_t elesz, void (*destroy)(void *));
```

Initializes an empty queue with element size `elesz` and an optional destructor. Must be called before any other function. Returns 0 on success, -1 on error.

**Parameters**

- `queue` — pointer to an uninitialized queue struct
- `elesz` — byte size of each element, must be non-zero
- `destroy` — called on each element when it is discarded, or NULL for no-op

---

### queue_fini

```c
void queue_fini(struct queue *queue);
```

Destroys all elements and frees the buffer. No-op if `queue` is NULL. Does not free the `struct queue` itself.

**Parameters**

- `queue` — pointer to the queue

---

### queue_enq

```c
int queue_enq(struct queue *queue, void *ele);
```

Copies `elesz` bytes from `ele` into a new slot at the back, growing the buffer if needed. Returns 0 on success, -1 on error.

**Parameters**

- `queue` — pointer to the queue
- `ele` — pointer to the value to copy, must not be NULL

---

### queue_deq

```c
int queue_deq(struct queue *queue, void *dest);
```

Removes the front element. If `dest` is non-NULL, copies the element there and skips `destroy`. If `dest` is NULL and `destroy` is set, calls `destroy` on the element. Returns 0 on success, -1 if the queue is empty or `queue` is NULL.

**Parameters**

- `queue` — pointer to the queue
- `dest` — destination buffer of at least `elesz` bytes to receive the element, or NULL to invoke `destroy`

---

### queue_peek

```c
void *queue_peek(struct queue *queue);
```

Returns a pointer to the front element, or NULL if the queue is empty or `queue` is NULL.

**Parameters**

- `queue` — pointer to the queue

---

### queue_clear

```c
void queue_clear(struct queue *queue);
```

Removes all elements and resets size to zero, calling `destroy` on each when set. Does not free the buffer or the `struct queue` itself. No-op if `queue` is NULL.

**Parameters**

- `queue` — pointer to the queue

---

## Example

```c
#include <queue.h>

int main(void)
{
  struct queue q;
  int x, y;

  if (queue_init(&q, sizeof(int), NULL) != 0)
    return 1;

  x = 10;
  queue_enq(&q, &x);
  x = 20;
  queue_enq(&q, &x);

  if (!queue_empty(&q))
    y = *(int *)queue_peek(&q); /* oldest value 10 */

  queue_deq(&q, &y); /* y == 10 */
  queue_deq(&q, &y); /* y == 20 */

  queue_clear(&q);
  queue_fini(&q);
  return 0;
}
```
</think>
Fixing the example: using standard `/* */` block comments like `stack.md` and `deque.md` (the `/_ _/` rule appears to mean block comments, not `//`).

<｜tool▁calls▁begin｜><｜tool▁call▁begin｜>
StrReplace