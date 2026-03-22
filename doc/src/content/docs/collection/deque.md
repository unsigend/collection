---
title: Deque
description: Double-ended queue of fixed-size elements with front and back operations
---

A deque stores elements in a linear order indexed from `0` at the front through `deq_size(deq) - 1` at the back. Each slot is exactly `elesz` bytes. You can push and pop at both ends, read by index with `deq_at`, resize and shrink the logical length, and insert or remove in the middle like a vector. Initialization and destructor rules match the same pattern as `vec_init` and related APIs.

## Header

```c
#include <deque.h>
```

## Struct

```c
struct deque {
  char *buf;
  size_t elesz;
  size_t sz;
  size_t cap;
  size_t head;
  void (*destroy)(void *);
};
```

`buf` is the element storage, `elesz` is the byte size of one element, `sz` is the current element count, `cap` is the number of allocated slots, `head` is layout metadata used with `buf` for logical ordering, `destroy` is the optional destructor from `deq_init`, or NULL if not set.

## Macros

### deq_empty

```c
deq_empty(deq)
```

Returns non-zero if the deque contains no elements.

**Parameters**

- `deq` — pointer to the deque

---

### deq_size

```c
deq_size(deq)
```

Returns the current element count.

**Parameters**

- `deq` — pointer to the deque

---

### deq_capacity

```c
deq_capacity(deq)
```

Returns the number of allocated element slots.

**Parameters**

- `deq` — pointer to the deque

---

### deq_front

```c
deq_front(deq)
```

Returns a pointer to the first element, equivalent to `deq_at(deq, 0)`. Returns NULL if the deque is empty.

**Parameters**

- `deq` — pointer to the deque

---

### deq_back

```c
deq_back(deq)
```

Returns a pointer to the last element, equivalent to `deq_at` at the last index. Returns NULL if the deque is empty.

**Parameters**

- `deq` — pointer to the deque

---

## Functions

### deq_init

```c
int deq_init(struct deque *deq, size_t elesz, void (*destroy)(void *));
```

Initializes an empty deque with element size `elesz` and an optional destructor. Must be called before any other function. Returns 0 on success, -1 on error.

**Parameters**

- `deq` — pointer to an uninitialized deque struct
- `elesz` — byte size of each element, must be non-zero
- `destroy` — called on each element when it is discarded, or NULL for no-op

---

### deq_fini

```c
void deq_fini(struct deque *deq);
```

Destroys all elements and frees the buffer. No-op if `deq` is NULL. Does not free the `struct deque` itself.

**Parameters**

- `deq` — pointer to the deque

---

### deq_at

```c
void *deq_at(const struct deque *deq, size_t idx);
```

Returns a pointer to the element at `idx`, or NULL if `deq` is NULL or `idx` is out of range.

**Parameters**

- `deq` — pointer to the deque
- `idx` — zero-based element index from front to back

---

### deq_resize

```c
int deq_resize(struct deque *deq, size_t newsize);
```

Sets the logical length to `newsize`. If `newsize` equals the current size this is a no-op. If `newsize` is greater, the buffer is reallocated if needed and new slots are left uninitialized. If `newsize` is smaller, `destroy` is called on each removed element when set. Returns 0 on success, -1 on error.

**Parameters**

- `deq` — pointer to the deque
- `newsize` — new element count

---

### deq_shrink

```c
int deq_shrink(struct deque *deq);
```

Reallocates the buffer to match the current size, releasing unused capacity. Returns immediately without reallocating if size is zero or already equals capacity. Returns 0 on success, -1 on error.

**Parameters**

- `deq` — pointer to the deque

---

### deq_insert

```c
int deq_insert(struct deque *deq, size_t idx, void *ele);
```

Inserts a copy of `ele` before `idx`, shifting subsequent elements toward the back. If `idx` is at or beyond the current size, behaves identically to `deq_pushback`. Returns 0 on success, -1 on error.

**Parameters**

- `deq` — pointer to the deque
- `idx` — insertion position
- `ele` — pointer to the value to copy, must not be NULL

---

### deq_remove

```c
int deq_remove(struct deque *deq, size_t idx, void *dest);
```

Removes the element at `idx`, shifting subsequent elements toward the front. If `dest` is non-NULL, copies the element there and skips `destroy`. If `dest` is NULL and `destroy` is set, calls `destroy` on the element. Returns 0 on success, -1 if `deq` is NULL or `idx` is out of range.

**Parameters**

- `deq` — pointer to the deque
- `idx` — index of the element to remove
- `dest` — destination buffer of at least `elesz` bytes to receive the element, or NULL to invoke `destroy`

---

### deq_pushback

```c
int deq_pushback(struct deque *deq, void *ele);
```

Copies `elesz` bytes from `ele` into a new slot at the back, growing the buffer if needed. Returns 0 on success, -1 on error.

**Parameters**

- `deq` — pointer to the deque
- `ele` — pointer to the value to copy, must not be NULL

---

### deq_pushfront

```c
int deq_pushfront(struct deque *deq, void *ele);
```

Copies `elesz` bytes from `ele` into a new slot at the front, growing the buffer if needed. Returns 0 on success, -1 on error.

**Parameters**

- `deq` — pointer to the deque
- `ele` — pointer to the value to copy, must not be NULL

---

### deq_popback

```c
int deq_popback(struct deque *deq, void *dest);
```

Removes the back element. If `dest` is non-NULL, copies the element there and skips `destroy`. If `dest` is NULL and `destroy` is set, calls `destroy` on the element. Returns 0 on success, -1 if the deque is empty or `deq` is NULL.

**Parameters**

- `deq` — pointer to the deque
- `dest` — destination buffer of at least `elesz` bytes to receive the element, or NULL to invoke `destroy`

---

### deq_popfront

```c
int deq_popfront(struct deque *deq, void *dest);
```

Removes the front element. If `dest` is non-NULL, copies the element there and skips `destroy`. If `dest` is NULL and `destroy` is set, calls `destroy` on the element. Returns 0 on success, -1 if the deque is empty or `deq` is NULL.

**Parameters**

- `deq` — pointer to the deque
- `dest` — destination buffer of at least `elesz` bytes to receive the element, or NULL to invoke `destroy`

---

### deq_clear

```c
void deq_clear(struct deque *deq);
```

Removes all elements and resets size to zero, calling `destroy` on each when set. Does not free the buffer or the `struct deque` itself. No-op if `deq` is NULL.

**Parameters**

- `deq` — pointer to the deque

---

## Example

```c
#include <deque.h>

int main(void)
{
  struct deque d;
  int x, y;

  if (deq_init(&d, sizeof(int), NULL) != 0)
    return 1;

  x = 1;
  deq_pushback(&d, &x);
  x = 2;
  deq_pushfront(&d, &x); /* order: 2, 1 */

  if (!deq_empty(&d))
    y = *(int *)deq_front(&d); /* y == 2 */

  deq_popfront(&d, &y);
  deq_popback(&d, &y); /* y == 1 */

  deq_clear(&d);
  deq_fini(&d);
  return 0;
}
```
