---
title: Set
description: Generic unique-element set backed by a hash table
---

A set stores unique opaque element pointers. Membership is defined by `hash` and `cmp` from `struct set_fns`, the same callbacks used for the underlying hash table. Elements are stored as keys only, `set_insert` ignores duplicates and returns 0, `set_remove` ignores missing elements and returns 0. Iteration with `struct set_iter` visits every element once, but visit order is not insertion order and is not sorted.

## Header

```c
#include <set.h>
```

## Structs

```c
struct set_fns {
  uint32_t (*hash)(void *);
  int (*cmp)(void *, void *);
  void (*destroy)(void *);
};
```

`hash` maps an element pointer to a hash code, `cmp` compares two elements and follows the usual negative, zero, positive convention, `destroy` may be NULL when the caller owns storage and needs no callback.

```c
struct set {
  struct hashtbl tbl;
  struct hashtbl_fns tbl_fns;
};
```

`tbl` is the backing hash table, `tbl_fns` is the internal callback bundle wired from `set_fns` at init.

```c
struct set_iter {
  struct hashtbl_iter iter;
};
```

`iter` tracks the current position for `set_iter_get` and `set_iter_inc`.

## Macros

### set_empty

```c
set_empty(set)
```

Evaluates to non-zero when the set has no elements.

**Parameters**

- `set` — pointer to the set

---

### set_size

```c
set_size(set)
```

Evaluates to the number of elements.

**Parameters**

- `set` — pointer to the set

---

## Functions

### set_init

```c
int set_init(struct set *set, struct set_fns *fns);
```

Prepares an empty set and binds `fns`. Must be called before any other operation on `set`. Returns 0 on success, -1 on error.

**Parameters**

- `set` — pointer to an uninitialized `struct set`
- `fns` — pointer to filled-in `struct set_fns`, only needed for the duration of the call

---

### set_fini

```c
void set_fini(struct set *set);
```

Tears down the set and releases all elements. No-op if `set` is NULL. Does not free the `struct set` itself.

**Parameters**

- `set` — pointer to the set

---

### set_insert

```c
int set_insert(struct set *set, void *ele);
```

Inserts `ele` when it is not already present. Returns 0 on success, -1 on error. If the element already exists, returns 0 without changing the set. `ele` must not be NULL.

**Parameters**

- `set` — pointer to the set
- `ele` — pointer stored as the element identity

---

### set_remove

```c
int set_remove(struct set *set, void *ele);
```

Removes `ele` when present. Returns 0 on success, -1 on error. If the element is not present, returns 0 without changing the set. `ele` must not be NULL. When `destroy` is set, the removed element is passed to it once.

**Parameters**

- `set` — pointer to the set
- `ele` — lookup element

---

### set_contains

```c
int set_contains(struct set *set, void *ele);
```

Returns non-zero if `ele` is in the set, 0 otherwise. Returns 0 if `set` or `ele` is NULL.

**Parameters**

- `set` — pointer to the set
- `ele` — lookup element

---

### set_clear

```c
void set_clear(struct set *set);
```

Removes every element, calling `destroy` on each when set. No-op if `set` is NULL or the set is already empty.

**Parameters**

- `set` — pointer to the set

---

### set_iter_init

```c
int set_iter_init(struct set_iter *iter, struct set *set);
```

Points `iter` at the first element, or NULL when the set is empty. After inserts or clears, call again so `iter` matches the current set. Returns 0 on success, -1 if `iter` or `set` is NULL.

**Parameters**

- `iter` — pointer to the iterator struct
- `set` — pointer to an initialized set

---

### set_iter_inc

```c
void set_iter_inc(struct set_iter *iter);
```

Advances `iter` to the next element. No-op if `iter` is NULL or the iterator is not on an element.

**Parameters**

- `iter` — pointer to the iterator

---

### set_iter_get

```c
void *set_iter_get(struct set_iter *iter);
```

Returns the current element pointer, or NULL if `iter` is NULL or the iterator is not on an element.

**Parameters**

- `iter` — pointer to the iterator

---

## Example

```c
#include <set.h>
#include <stdint.h>

static uint32_t hash_int(void *k)
{
  return (uint32_t)(*(int *)k);
}

static int cmp_int(void *a, void *b)
{
  int ka = *(int *)a;
  int kb = *(int *)b;
  if (ka < kb)
    return -1;
  if (ka > kb)
    return 1;
  return 0;
}

int main(void)
{
  struct set s;
  struct set_fns fns = {hash_int, cmp_int, NULL};
  struct set_iter it;
  int a, b, c;
  void *ele;

  if (set_init(&s, &fns) != 0)
    return 1;

  a = 1;
  b = 2;
  c = 3;
  set_insert(&s, &a);
  set_insert(&s, &b);
  set_insert(&s, &c);
  set_insert(&s, &a); /* duplicate, no-op */

  if (set_contains(&s, &b))
    set_remove(&s, &b);

  if (set_iter_init(&it, &s) == 0) {
    while ((ele = set_iter_get(&it)) != NULL) {
      (void)*(int *)ele;
      set_iter_inc(&it);
    }
  }

  set_clear(&s);
  set_fini(&s);
  return 0;
}
```
