---
title: Sort
description: Generic in-place array sort and optional raw algorithm entry points from sort.h
---

The sort API sorts `n` objects of `sz` bytes each in memory starting at `base`, using a caller-supplied comparator like `strcmp`. The library entry point you should use in most cases is `sort`, which is the default implementation described in the header as introsort-based.

## Header

```c
#include <sort.h>
```

## Optional macro

### COL_ALL_SORTS

If you define the macro `COL_ALL_SORTS` before including `sort.h`, the header also declares `sortins`, `sortqs`, and `sortheap`. The header describes these as internal raw sorts without the same tuning as the default, so they are not recommended for general use, prefer `sort` unless you have a specific reason to call a particular variant.

---

## Functions

### sort

```c
int sort(void *base, size_t n, size_t sz, int (*cmp)(void *, void *));
```

Sorts the `n` elements in place. Each element occupies `sz` bytes, `cmp` receives pointers to two elements and should return less than zero, zero, or greater than zero depending on their order, in the usual style of `strcmp`.

**Parameters**

- `base` — address of the first element
- `n` — number of elements
- `sz` — size of each element in bytes
- `cmp` — comparator

---

### sortins

Only declared when `COL_ALL_SORTS` is defined before including `sort.h`.

```c
int sortins(void *base, size_t n, size_t sz, int (*cmp)(void *, void *));
```

Insertion sort. Same parameters as `sort`. Prefer `sort` for ordinary use.

**Parameters**

- `base` — address of the first element
- `n` — number of elements
- `sz` — size of each element in bytes
- `cmp` — comparator

---

### sortqs

Only declared when `COL_ALL_SORTS` is defined before including `sort.h`.

```c
int sortqs(void *base, size_t n, size_t sz, int (*cmp)(void *, void *));
```

Quicksort. Same parameters as `sort`. Prefer `sort` for ordinary use.

**Parameters**

- `base` — address of the first element
- `n` — number of elements
- `sz` — size of each element in bytes
- `cmp` — comparator

---

### sortheap

Only declared when `COL_ALL_SORTS` is defined before including `sort.h`.

```c
int sortheap(void *base, size_t n, size_t sz, int (*cmp)(void *, void *));
```

Heap sort. Same parameters as `sort`. Prefer `sort` for ordinary use.

**Parameters**

- `base` — address of the first element
- `n` — number of elements
- `sz` — size of each element in bytes
- `cmp` — comparator

---

## Example

```c
#include <sort.h>

static int cmp_int(void *a, void *b)
{
  int x = *(int *)a;
  int y = *(int *)b;
  if (x < y)
    return -1;
  if (x > y)
    return 1;
  return 0;
}

void sort_three_ints(int *buf)
{
  sort(buf, 3, sizeof *buf, cmp_int);
}
```
