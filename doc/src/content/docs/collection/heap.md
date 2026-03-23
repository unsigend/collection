---
title: Heap
description: Priority queue of fixed-size elements backed by a growable vector
---

A heap stores `elesz`-byte elements in an array-based tree. The comparator from `heap_init` decides which element has highest priority, that element is read with `heap_peek` and removed with `heap_pop`. New values are added with `heap_push`.

## Header

```c
#include <heap.h>
```

## Struct

```c
struct heap {
  struct vector vec;
  int (*cmp)(void *, void *);
};
```

`vec` holds the element buffer and metadata from initialization, `cmp` is the comparator passed to `heap_init` and used for ordering.

## Macros

### heap_empty

```c
heap_empty(heap)
```

Returns non-zero if the heap contains no elements.

**Parameters**

- `heap` — pointer to the heap

---

### heap_size

```c
heap_size(heap)
```

Returns the current element count.

**Parameters**

- `heap` — pointer to the heap

---

## Functions

### heap_init

```c
int heap_init(struct heap *heap, size_t elesz, int (*cmp)(void *, void *),
              void (*destroy)(void *));
```

Prepares an empty heap with element size `elesz`, comparator `cmp`, and optional `destroy`. Must be called before any other heap function. Returns 0 on success, -1 on error.

**Parameters**

- `heap` — pointer to an uninitialized heap struct
- `elesz` — byte size of each element, must be non-zero
- `cmp` — compares two elements, each argument points to `elesz` bytes in heap storage, return less than zero, zero, or greater than zero like `strcmp` to express ordering
- `destroy` — called when the vector discards an element slot, or NULL for no-op

---

### heap_fini

```c
void heap_fini(struct heap *heap);
```

Destroys all elements and frees the underlying buffer. No-op if `heap` is NULL. Does not free the `struct heap` itself.

**Parameters**

- `heap` — pointer to the heap

---

### heap_push

```c
int heap_push(struct heap *heap, void *ele);
```

Copies `elesz` bytes from `ele` into the heap and restores the ordering defined by `cmp`. Returns 0 on success, -1 on error.

**Parameters**

- `heap` — pointer to the heap
- `ele` — pointer to the value to copy, must not be NULL

---

### heap_pop

```c
int heap_pop(struct heap *heap, void *dest);
```

Removes the first element according to `cmp`. If `dest` is non-NULL, copies the element there and skips `destroy` for that removal in the usual vector sense. If `dest` is NULL and `destroy` is set, the slot is released via `destroy`. Returns 0 on success, -1 if `heap` is NULL or the heap is empty.

**Parameters**

- `heap` — pointer to the heap
- `dest` — destination buffer of at least `elesz` bytes, or NULL to invoke `destroy` when configured

---

### heap_peek

```c
void *heap_peek(struct heap *heap);
```

Returns a pointer to the first element according to `cmp`, or NULL if `heap` is NULL or the heap is empty.

**Parameters**

- `heap` — pointer to the heap

---

### heap_clear

```c
void heap_clear(struct heap *heap);
```

Removes all elements and resets the size to zero, calling `destroy` on each when set. Does not free the `struct heap` itself. No-op if `heap` is NULL.

**Parameters**

- `heap` — pointer to the heap

---

## Example

```c
#include <heap.h>

static int
cmp_int(void *a, void *b)
{
  int x = *(int *)a;
  int y = *(int *)b;
  return (x > y) - (x < y);
}

int
main(void)
{
  struct heap h;
  int a, b, c, out;

  if (heap_init(&h, sizeof(int), cmp_int, NULL) != 0)
    return 1;

  a = 3;
  b = 1;
  c = 2;
  heap_push(&h, &a);
  heap_push(&h, &b);
  heap_push(&h, &c);

  if (!heap_empty(&h))
    out = *(int *)heap_peek(&h); /* smallest by cmp_int */

  heap_pop(&h, &out); /* out == 1 */
  heap_pop(&h, &out);
  heap_pop(&h, &out);

  heap_clear(&h);
  heap_fini(&h);
  return 0;
}
```
