---
title: Hash
description: Hash helpers for integer and string keys, declared in hash.h
---

The hash algorithms module provides simple hash entry points for common key types, declarations are in `hash.h`.

## Header

```c
#include <hash.h>
```

## Functions

### hash_u32

```c
uint32_t hash_u32(uint32_t key);
```

Computes a hash value from a 32 bit unsigned integer key.

**Parameters**

- `key` - input unsigned 32 bit key

**Return value**

- hash result as `uint32_t`

### hash_u64

```c
uint32_t hash_u64(uint64_t key);
```

Computes a hash value from a 64 bit unsigned integer key.

**Parameters**

- `key` - input unsigned 64 bit key

**Return value**

- hash result as `uint32_t`

### hash_str

```c
uint32_t hash_str(const char *str);
```

Computes a hash value from a null terminated string.

**Parameters**

- `str` - pointer to input string data

**Return value**

- hash result as `uint32_t`
