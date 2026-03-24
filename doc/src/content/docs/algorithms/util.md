---
title: Util
description: Generic helpers for collection algorithms, declared in util.h
---

The util algorithms module is for small, reusable primitives that other parts of the library can build on, declarations are in `util.h`.

## Header

```c
#include <util.h>
```

## Functions

### swap

```c
void swap(void *l, void *r, size_t sz);
```

Reads `sz` bytes starting at `l` and `sz` bytes starting at `r`, writes each block into the other location, so the two regions end up with each other's prior contents. If `sz` is zero, neither pointer is accessed for swapping.

**Parameters**

- `l` — pointer to the first object
- `r` — pointer to the second object
- `sz` — number of bytes to exchange, usually `sizeof` the value type
