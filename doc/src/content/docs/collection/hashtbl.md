---
title: Hash table
description: Generic hash table with chaining and callback-driven key and value lifetime
---

A hash table maps caller-supplied keys to values using a `struct hashtbl_fns` bundle, hash and comparison functions are required, optional destructors can release owned key or value memory when entries are removed or replaced. Keys and values are opaque pointers, the table stores those pointers rather than copying pointed-to data. Iteration with `struct hashtbl_iter` visits every entry once, but bucket order and chaining mean visit order is not insertion order and is not sorted by key.

## Header

```c
#include <hashtbl.h>
```

## Structs

```c
struct hashtbl_node {
  void *key;
  void *val;
  struct hashtbl_node *next;
};
```

Each node holds one key pointer, one value pointer, and a `next` link for entries that share the same bucket.

```c
struct hashtbl_fns {
  uint32_t (*hash)(void *);
  int (*cmp)(void *, void *);
  void (*destroy_key)(void *);
  void (*destroy_val)(void *);
};
```

`hash` maps a key pointer to a hash code, `cmp` compares two keys and follows the usual negative, zero, positive convention, `destroy_key` and `destroy_val` may be NULL when the caller owns storage and needs no callback.

```c
struct hashtbl {
  struct hashtbl_node **buckets;
  size_t bucketsz;
  size_t sz;
  float threshold; /* max load factor */
  struct hashtbl_fns *fns;
};
```

`buckets` is the bucket array, `bucketsz` is its length, `sz` is the number of entries, `threshold` is the configured maximum load factor, `fns` points to the callback bundle passed to `hashtbl_init`.

```c
struct hashtbl_iter {
  struct hashtbl *ht;
  size_t bucket;
  struct hashtbl_node *node;
};
```

`ht` is the table being traversed, `bucket` and `node` locate the current chain link for `hashtbl_iter_get` and `hashtbl_iter_inc`.

## Macros

### hashtbl_empty

```c
hashtbl_empty(ht)
```

Evaluates to non-zero when the table has no entries.

**Parameters**

- `ht` — pointer to the hash table

---

### hashtbl_size

```c
hashtbl_size(ht)
```

Evaluates to the number of entries.

**Parameters**

- `ht` — pointer to the hash table

---

### hashtbl_threshold

```c
hashtbl_threshold(ht)
```

Evaluates to the current maximum load factor value.

**Parameters**

- `ht` — pointer to the hash table

---

### hashtbl_bucketsz

```c
hashtbl_bucketsz(ht)
```

Evaluates to the current bucket array length.

**Parameters**

- `ht` — pointer to the hash table

---

### hashtbl_buckets

```c
hashtbl_buckets(ht)
```

Evaluates to the bucket pointer array.

**Parameters**

- `ht` — pointer to the hash table

---

### hashtbl_fns

```c
hashtbl_fns(ht)
```

Evaluates to the pointer to `struct hashtbl_fns` installed at init.

**Parameters**

- `ht` — pointer to the hash table

---

## Functions

### hashtbl_init

```c
int hashtbl_init(struct hashtbl *ht, struct hashtbl_fns *fns);
```

Prepares an empty table and binds `fns`. Must be called before any other operation on `ht`. Returns 0 on success, -1 on error.

**Parameters**

- `ht` — pointer to an uninitialized `struct hashtbl`
- `fns` — pointer to filled-in `struct hashtbl_fns`, must remain valid for the lifetime of the table

---

### hashtbl_fini

```c
void hashtbl_fini(struct hashtbl *ht);
```

Tears down the table and releases all entries. No-op if `ht` is NULL. Does not free the `struct hashtbl` itself.

**Parameters**

- `ht` — pointer to the hash table

---

### hashtbl_setthreshold

```c
int hashtbl_setthreshold(struct hashtbl *ht, float threshold, float *old);
```

Sets the maximum load factor. Returns 0 on success, -1 on error. If `old` is non-NULL, writes the previous threshold value there.

**Parameters**

- `ht` — pointer to the hash table
- `threshold` — new maximum load factor
- `old` — optional output for the previous threshold, or NULL

---

### hashtbl_loadfactor

```c
float hashtbl_loadfactor(struct hashtbl *ht);
```

Returns the current load factor.

**Parameters**

- `ht` — pointer to the hash table

---

### hashtbl_insert

```c
int hashtbl_insert(struct hashtbl *ht, void *key, void *val);
```

Inserts a new key and value. The key must not already be present. Returns 0 on success, -1 on error or if the key already exists. `key` must not be NULL, `val` may be NULL.

**Parameters**

- `ht` — pointer to the hash table
- `key` — pointer stored as the key identity
- `val` — pointer stored as the value, may be NULL

---

### hashtbl_update

```c
int hashtbl_update(struct hashtbl *ht, void *key, void *newval, void **dest);
```

Replaces the value for an existing key. Returns 0 on success, -1 on error or if the key is not found. `newval` must not be NULL.

**Parameters**

- `ht` — pointer to the hash table
- `key` — lookup key
- `newval` — pointer to store as the new value
- `dest` — if non-NULL, receives the previous value pointer, otherwise the old value may be passed to `destroy_val` when that callback is set

---

### hashtbl_remove

```c
int hashtbl_remove(struct hashtbl *ht, void *key, void **dest);
```

Removes the entry for `key`. Returns 0 on success, -1 on error or if the key is not found. If `dest` is non-NULL, copies the removed value pointer there and skips `destroy_val` on that value, otherwise the value may be destroyed when `destroy_val` is set. The key may be destroyed when `destroy_key` is set.

**Parameters**

- `ht` — pointer to the hash table
- `key` — lookup key
- `dest` — optional output for the removed value pointer, or NULL

---

### hashtbl_find

```c
void *hashtbl_find(struct hashtbl *ht, void *key);
```

Returns the pointer stored as the value for `key`, or NULL if there is no such entry.

**Parameters**

- `ht` — pointer to the hash table
- `key` — lookup key

---

### hashtbl_findnode

```c
struct hashtbl_node *hashtbl_findnode(struct hashtbl *ht, void *key);
```

Returns the node for `key`, or NULL if there is no such entry.

**Parameters**

- `ht` — pointer to the hash table
- `key` — lookup key

---

### hashtbl_clear

```c
void hashtbl_clear(struct hashtbl *ht);
```

Removes every entry. No-op if `ht` is NULL or the table is already empty.

**Parameters**

- `ht` — pointer to the hash table

---

### hashtbl_iter_init

```c
int hashtbl_iter_init(struct hashtbl_iter *iter, struct hashtbl *ht);
```

Points `iter` at the head of bucket `0`, or NULL when that bucket has no nodes or `ht` has no bucket array yet. After inserts or clears, call again so `iter` matches the current table. Returns 0 on success, -1 if `iter` or `ht` is NULL.

**Parameters**

- `iter` — pointer to the iterator struct
- `ht` — pointer to an initialized table

---

### hashtbl_iter_inc

```c
void hashtbl_iter_inc(struct hashtbl_iter *iter);
```

Advances `iter` along the current bucket chain, then continues at the next bucket when the chain ends. No-op if `iter` is NULL or the iterator is not on a node.

**Parameters**

- `iter` — pointer to the iterator

---

### hashtbl_iter_get

```c
struct hashtbl_node *hashtbl_iter_get(struct hashtbl_iter *iter);
```

Returns the current `struct hashtbl_node`, or NULL if `iter` is NULL or the iterator is not on an entry.

**Parameters**

- `iter` — pointer to the iterator

---

## Example

```c
#include <hashtbl.h>
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
  struct hashtbl ht;
  struct hashtbl_fns fns = {hash_int, cmp_int, NULL, NULL};
  int k, v;
  void *pv;

  if (hashtbl_init(&ht, &fns) != 0)
    return 1;

  k = 42;
  v = 100;
  if (hashtbl_insert(&ht, &k, &v) != 0) {
    hashtbl_fini(&ht);
    return 1;
  }

  pv = hashtbl_find(&ht, &k);
  if (pv != NULL)
    v = *(int *)pv;

  hashtbl_remove(&ht, &k, NULL);
  hashtbl_fini(&ht);
  return 0;
}
```
