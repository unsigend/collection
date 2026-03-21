---
title: Vector
description: Contiguous resizable array of fixed-size elements
---

A vector stores elements in a single contiguous buffer where each slot is exactly `elesz` bytes. Random access is O(1) via `vec_at`, growth happens automatically on push and insert, shrink is explicit.

## Header

```c
#include <vector.h>
```

## Struct

```c
struct vector {
  char *buf;
  size_t elesz;
  size_t sz;
  size_t cap;
  void (*destroy)(void *);
};
```

`buf` is the element buffer, `elesz` is the byte size of one element, `sz` is the current element count, `cap` is the number of allocated slots, `destroy` is the optional destructor passed to `vec_init`, or NULL if not set.

## Macros

### vec_empty

```c
vec_empty(vec)
```

Returns non-zero if the vector contains no elements.

**Parameters**

- `vec` — pointer to the vector

---

### vec_raw

```c
vec_raw(vec)
```

Returns the raw buffer pointer for the element slab.

**Parameters**

- `vec` — pointer to the vector

---

### vec_size

```c
vec_size(vec)
```

Returns the current element count.

**Parameters**

- `vec` — pointer to the vector

---

### vec_capacity

```c
vec_capacity(vec)
```

Returns the number of allocated element slots.

**Parameters**

- `vec` — pointer to the vector

---

### vec_front

```c
vec_front(vec)
```

Returns a pointer to the first element, equivalent to `vec_at(vec, 0)`. Returns NULL if the vector is empty.

**Parameters**

- `vec` — pointer to the vector

---

### vec_back

```c
vec_back(vec)
```

Returns a pointer to the last element, equivalent to `vec_at` at the last index. Returns NULL if the vector is empty.

**Parameters**

- `vec` — pointer to the vector

---

## Functions

### vec_init

```c
int vec_init(struct vector *vec, size_t elesz, void (*destroy)(void *));
```

Initializes an empty vector with element size `elesz` and an optional destructor. Must be called before any other function. Returns 0 on success, -1 on error.

**Parameters**

- `vec` — pointer to an uninitialized vector struct
- `elesz` — byte size of each element, must be non-zero
- `destroy` — called on each element when it is discarded, or NULL for no-op

---

### vec_fini

```c
void vec_fini(struct vector *vec);
```

Destroys all elements and frees the buffer. No-op if `vec` is NULL. Does not free the `struct vector` itself.

**Parameters**

- `vec` — pointer to the vector

---

### vec_at

```c
void *vec_at(const struct vector *vec, size_t idx);
```

Returns a pointer to the element at `idx`, or NULL if `vec` is NULL or `idx` is out of range.

**Parameters**

- `vec` — pointer to the vector
- `idx` — zero-based element index

---

### vec_resize

```c
int vec_resize(struct vector *vec, size_t newsz);
```

Sets the logical length to `newsz`. If `newsz` equals the current size this is a no-op. If `newsz` is greater, the buffer is reallocated if needed and new slots are left uninitialized. If `newsz` is smaller, `destroy` is called on each removed element when set. Returns 0 on success, -1 on error.

**Parameters**

- `vec` — pointer to the vector
- `newsz` — new element count

---

### vec_shrink

```c
int vec_shrink(struct vector *vec);
```

Reallocates the buffer to match the current size, releasing unused capacity. Returns immediately without reallocating if size is zero or already equals capacity. Returns 0 on success, -1 on error.

**Parameters**

- `vec` — pointer to the vector

---

### vec_pushback

```c
int vec_pushback(struct vector *vec, void *ele);
```

Copies `elesz` bytes from `ele` into a new slot at the end, growing the buffer if needed. Returns 0 on success, -1 on error.

**Parameters**

- `vec` — pointer to the vector
- `ele` — pointer to the value to copy, must not be NULL

---

### vec_popback

```c
int vec_popback(struct vector *vec, void *dest);
```

Removes the last element. If `dest` is non-NULL, copies the element there and skips `destroy`. If `dest` is NULL and `destroy` is set, calls `destroy` on the element. Returns 0 on success, -1 if the vector is empty or `vec` is NULL.

**Parameters**

- `vec` — pointer to the vector
- `dest` — destination buffer of at least `elesz` bytes to receive the element, or NULL to invoke `destroy`

---

### vec_insert

```c
int vec_insert(struct vector *vec, size_t idx, void *ele);
```

Inserts a copy of `ele` before `idx`, shifting subsequent elements right. If `idx` is at or beyond the current size, behaves identically to `vec_pushback`. Returns 0 on success, -1 on error.

**Parameters**

- `vec` — pointer to the vector
- `idx` — insertion position
- `ele` — pointer to the value to copy, must not be NULL

---

### vec_remove

```c
int vec_remove(struct vector *vec, size_t idx, void *dest);
```

Removes the element at `idx`, shifting subsequent elements left. If `dest` is non-NULL, copies the element there and skips `destroy`. If `dest` is NULL and `destroy` is set, calls `destroy` on the element. Returns 0 on success, -1 if `vec` is NULL or `idx` is out of range.

**Parameters**

- `vec` — pointer to the vector
- `idx` — index of the element to remove
- `dest` — destination buffer of at least `elesz` bytes to receive the element, or NULL to invoke `destroy`

---

### vec_clear

```c
void vec_clear(struct vector *vec);
```

Removes all elements and resets size to zero, calling `destroy` on each when set. Does not free the buffer or the `struct vector` itself. No-op if `vec` is NULL.

**Parameters**

- `vec` — pointer to the vector

---

## Example

```c
#include <vector.h>

int main(void)
{
  struct vector v;
  int x;

  /* initialize a vector of int with no destructor */
  if (vec_init(&v, sizeof(int), NULL) != 0)
    return 1;

  /* append two elements */
  x = 1;
  vec_pushback(&v, &x);
  x = 2;
  vec_pushback(&v, &x);

  /* read front element */
  if (!vec_empty(&v))
    x = *(int *)vec_front(&v); /* x == 1 */

  vec_clear(&v);
  vec_fini(&v);
  return 0;
}
```
