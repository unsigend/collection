---
title: AVL tree
description: Ordered map of caller-supplied keys to values with comparison-driven lookup and optional key and value callbacks
---

An AVL tree keeps unique keys ordered by `cmp` from `struct avltree_fns`. Keys and values are opaque pointers, the structure stores those pointers rather than copying pointed-to data. The header positions this type for read-heavy workloads.

## Header

```c
#include <avltree.h>
```

## Structs

```c
struct avltree_node {
  void *key;
  void *val;
  int height;
  struct avltree_node *left;
  struct avltree_node *right;
};
```

Each node holds one key pointer, one value pointer, subtree height, and left and right child pointers.

```c
struct avltree_fns {
  int (*cmp)(void *, void *);
  void (*destroy_key)(void *);
  void (*destroy_val)(void *);
};
```

`cmp` compares two keys and follows the usual negative, zero, positive convention, `destroy_key` and `destroy_val` may be NULL when the caller owns storage and needs no callback.

```c
struct avltree {
  struct avltree_node *root;
  size_t size;
  struct avltree_fns *fns;
};
```

`root` is the tree root, `size` counts entries, `fns` points to the callback bundle passed to `avltree_init`.

## Macros

### avltree_empty

```c
avltree_empty(tree)
```

Evaluates to non-zero when the tree has no entries.

**Parameters**

- `tree` — pointer to the tree

---

### avltree_size

```c
avltree_size(tree)
```

Evaluates to the number of entries.

**Parameters**

- `tree` — pointer to the tree

---

### avltree_root

```c
avltree_root(tree)
```

Evaluates to the root node pointer.

**Parameters**

- `tree` — pointer to the tree

---

### avltree_fns

```c
avltree_fns(tree)
```

Evaluates to the pointer to `struct avltree_fns` installed at init.

**Parameters**

- `tree` — pointer to the tree

---

## Functions

### avltree_init

```c
int avltree_init(struct avltree *tree, struct avltree_fns *fns);
```

Prepares an empty tree and binds `fns`. Must be called before any other operation on `tree`. Returns 0 on success, -1 on error.

**Parameters**

- `tree` — pointer to an uninitialized `struct avltree`
- `fns` — pointer to filled-in `struct avltree_fns`, must remain valid for the lifetime of the tree

---

### avltree_fini

```c
void avltree_fini(struct avltree *tree);
```

Tears down the tree and releases all entries. Does not free the `struct avltree` itself.

**Parameters**

- `tree` — pointer to the tree

---

### avltree_insert

```c
int avltree_insert(struct avltree *tree, void *key, void *val);
```

Inserts a new key and value. The key must not already be present. Returns 0 on success, -1 on error or if the key already exists.

**Parameters**

- `tree` — pointer to the tree
- `key` — pointer stored as the key identity
- `val` — pointer stored as the value

---

### avltree_update

```c
int avltree_update(struct avltree *tree, void *key, void *newval, void **dest);
```

Replaces the value for an existing key. Returns 0 on success, -1 on error or if the key is not found.

**Parameters**

- `tree` — pointer to the tree
- `key` — lookup key
- `newval` — pointer to store as the new value
- `dest` — optional output pointer for the previous value, or NULL

---

### avltree_remove

```c
int avltree_remove(struct avltree *tree, void *key, void **dest);
```

Removes the entry for `key`. Returns 0 on success, -1 on error or if the key is not found.

**Parameters**

- `tree` — pointer to the tree
- `key` — lookup key
- `dest` — optional output pointer for the removed value, or NULL

---

### avltree_find

```c
void *avltree_find(struct avltree *tree, void *key);
```

Returns the value pointer stored for `key`, or NULL if there is no such entry.

**Parameters**

- `tree` — pointer to the tree
- `key` — lookup key

---

### avltree_findnode

```c
struct avltree_node *avltree_findnode(struct avltree *tree, void *key);
```

Returns the node for `key`, or NULL if there is no such entry.

**Parameters**

- `tree` — pointer to the tree
- `key` — lookup key

---

### avltree_clear

```c
void avltree_clear(struct avltree *tree);
```

Removes every entry.

**Parameters**

- `tree` — pointer to the tree

---

## Example

```c
#include <avltree.h>

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
  struct avltree t;
  struct avltree_fns fns = {cmp_int, NULL, NULL};
  int k, v;
  void *pv;

  if (avltree_init(&t, &fns) != 0)
    return 1;

  k = 42;
  v = 100;
  if (avltree_insert(&t, &k, &v) != 0) {
    avltree_fini(&t);
    return 1;
  }

  pv = avltree_find(&t, &k);
  if (pv != NULL)
    v = *(int *)pv;

  avltree_remove(&t, &k, NULL);
  avltree_fini(&t);
  return 0;
}
```
