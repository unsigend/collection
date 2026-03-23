---
title: Doubly Linked List
description: Pointer based doubly linked list with node level insertion and removal
---

A dlist stores `void *` data pointers in doubly linked nodes, it supports push and pop from both ends, and it supports insertion or removal with node based APIs. Use `dlist_init` before other operations, use `dlist_fini` when done.

## Header

```c
#include <dlist.h>
```

## Struct

```c
struct dlist {
  struct dlist_node *head;
  struct dlist_node *tail;
  size_t len;
  void (*destroy)(void *);
};

struct dlist_node {
  void *data;
  struct dlist_node *next;
  struct dlist_node *prev;
};
```

`head` points to the first node, `tail` points to the last node, `len` is the node count, `destroy` is the optional callback used when removed data is not returned.

## Macros

### dlist_empty

```c
dlist_empty(dlist)
```

Returns non zero when the list has no nodes.

**Parameters**

- `dlist` - pointer to the list

---

### dlist_size

```c
dlist_size(dlist)
```

Returns current node count.

**Parameters**

- `dlist` - pointer to the list

---

### dlist_front

```c
dlist_front(dlist)
```

Returns data pointer from head node, returns NULL when the list is empty.

**Parameters**

- `dlist` - pointer to the list

---

### dlist_back

```c
dlist_back(dlist)
```

Returns data pointer from tail node, returns NULL when the list is empty.

**Parameters**

- `dlist` - pointer to the list

---

### dlist_next

```c
dlist_next(node)
```

Returns next node pointer, returns NULL when `node` is NULL.

**Parameters**

- `node` - pointer to a list node

---

### dlist_prev

```c
dlist_prev(node)
```

Returns previous node pointer, returns NULL when `node` is NULL.

**Parameters**

- `node` - pointer to a list node

---

### dlist_data

```c
dlist_data(node)
```

Returns data pointer stored in a node, returns NULL when `node` is NULL.

**Parameters**

- `node` - pointer to a list node

---

## Functions

### dlist_init

```c
int dlist_init(struct dlist *dlist, void (*destroy)(void *));
```

Initializes an empty list and sets optional destructor callback, returns 0 on success, -1 on error.

**Parameters**

- `dlist` - pointer to list struct
- `destroy` - callback for data cleanup, or NULL

---

### dlist_fini

```c
void dlist_fini(struct dlist *dlist);
```

Clears all nodes from the list, no op when `dlist` is NULL.

**Parameters**

- `dlist` - pointer to list struct

---

### dlist_pushfront

```c
int dlist_pushfront(struct dlist *dlist, void *data);
```

Inserts one data pointer at head, returns 0 on success, -1 on error.

**Parameters**

- `dlist` - pointer to list struct
- `data` - data pointer to store, must not be NULL

---

### dlist_pushback

```c
int dlist_pushback(struct dlist *dlist, void *data);
```

Inserts one data pointer at tail, returns 0 on success, -1 on error.

**Parameters**

- `dlist` - pointer to list struct
- `data` - data pointer to store, must not be NULL

---

### dlist_popfront

```c
int dlist_popfront(struct dlist *dlist, void **dest);
```

Removes head node, if `dest` is not NULL stores removed data pointer into `*dest`, if `dest` is NULL and destroy callback exists then callback is called for removed data, returns 0 on success, -1 on error.

**Parameters**

- `dlist` - pointer to list struct
- `dest` - output location for removed data pointer, or NULL

---

### dlist_popback

```c
int dlist_popback(struct dlist *dlist, void **dest);
```

Removes tail node, if `dest` is not NULL stores removed data pointer into `*dest`, if `dest` is NULL and destroy callback exists then callback is called for removed data, returns 0 on success, -1 on error.

**Parameters**

- `dlist` - pointer to list struct
- `dest` - output location for removed data pointer, or NULL

---

### dlist_insert

```c
int dlist_insert(struct dlist *dlist, struct dlist_node *node, void *data);
```

Inserts data pointer before `node`, when `node` is NULL inserts at head, returns 0 on success, -1 on error.

**Parameters**

- `dlist` - pointer to list struct
- `node` - node before which new node is inserted, or NULL for head insertion
- `data` - data pointer to store, must not be NULL

---

### dlist_remove

```c
int dlist_remove(struct dlist *dlist, struct dlist_node *node, void **dest);
```

Removes `node`, when `node` is NULL removes from head, if `dest` is not NULL stores removed data pointer into `*dest`, if `dest` is NULL and destroy callback exists then callback is called for removed data, returns 0 on success, -1 on error.

**Parameters**

- `dlist` - pointer to list struct
- `node` - node to remove, or NULL to remove from head
- `dest` - output location for removed data pointer, or NULL

---

### dlist_clear

```c
void dlist_clear(struct dlist *dlist);
```

Removes all nodes, calls destroy callback for each node data when callback exists, no op when `dlist` is NULL.

**Parameters**

- `dlist` - pointer to list struct

---

## Example

```c
#include <dlist.h>

int main(void)
{
  struct dlist d;
  int a, b;
  void *out;

  if (dlist_init(&d, NULL) != 0)
    return 1;

  a = 10;
  b = 20;
  dlist_pushback(&d, &a);
  dlist_pushfront(&d, &b); /* order is 20 then 10 */

  out = NULL;
  dlist_popfront(&d, &out);

  dlist_clear(&d);
  dlist_fini(&d);
  return 0;
}
```
