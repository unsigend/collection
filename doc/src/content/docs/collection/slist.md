---
title: Slist
description: Pointer based singly linked list with node level operations
---

An slist stores `void *` data pointers in forward linked nodes, it supports push and pop at list ends visible through front and back accessors, and node level insert or remove after a given node. Use `slist_init` before other operations, use `slist_fini` when done.

## Header

```c
#include <slist.h>
```

## Struct

```c
struct slist {
  struct slist_node *head;
  struct slist_node *tail;
  size_t len;
  void (*destroy)(void *);
};

struct slist_node {
  void *data;
  struct slist_node *next;
};
```

`head` points to the first node, `tail` points to the last node, `len` is the node count, `destroy` is the optional callback used when removed data is not returned to the caller.

## Macros

### slist_empty

```c
slist_empty(slist)
```

Returns non zero when the list has no nodes.

**Parameters**

- `slist` - pointer to the list

---

### slist_size

```c
slist_size(slist)
```

Returns current node count.

**Parameters**

- `slist` - pointer to the list

---

### slist_front

```c
slist_front(slist)
```

Returns data pointer from head node, returns NULL when the list is empty.

**Parameters**

- `slist` - pointer to the list

---

### slist_back

```c
slist_back(slist)
```

Returns data pointer from tail node, returns NULL when the list is empty.

**Parameters**

- `slist` - pointer to the list

---

### slist_next

```c
slist_next(node)
```

Returns next node pointer, returns NULL when `node` is NULL.

**Parameters**

- `node` - pointer to a list node

---

### slist_data

```c
slist_data(node)
```

Returns data pointer stored in a node, returns NULL when `node` is NULL.

**Parameters**

- `node` - pointer to a list node

---

## Functions

### slist_init

```c
int slist_init(struct slist *slist, void (*destroy)(void *));
```

Initializes an empty list, sets optional destructor callback, returns 0 on success, -1 on error.

**Parameters**

- `slist` - pointer to list struct
- `destroy` - callback for data cleanup, or NULL

---

### slist_fini

```c
void slist_fini(struct slist *slist);
```

Clears all nodes from the list, no op when `slist` is NULL.

**Parameters**

- `slist` - pointer to list struct

---

### slist_pushfront

```c
int slist_pushfront(struct slist *slist, void *data);
```

Inserts one data pointer at the head, returns 0 on success, -1 on error.

**Parameters**

- `slist` - pointer to list struct
- `data` - data pointer to store, must not be NULL

---

### slist_pushback

```c
int slist_pushback(struct slist *slist, void *data);
```

Inserts one data pointer at the tail, returns 0 on success, -1 on error.

**Parameters**

- `slist` - pointer to list struct
- `data` - data pointer to store, must not be NULL

---

### slist_popfront

```c
int slist_popfront(struct slist *slist, void **dest);
```

Removes head node, if `dest` is not NULL stores removed data pointer into `*dest`, if `dest` is NULL and destroy callback exists then callback is called for removed data, returns 0 on success, -1 on error.

**Parameters**

- `slist` - pointer to list struct
- `dest` - output location for removed data pointer, or NULL

---

### slist_insertn

```c
int slist_insertn(struct slist *slist, struct slist_node *node, void *data);
```

Inserts data pointer after `node`, when `node` is NULL inserts at head, returns 0 on success, -1 on error.

**Parameters**

- `slist` - pointer to list struct
- `node` - node after which new node is inserted, or NULL for head insertion
- `data` - data pointer to store, must not be NULL

---

### slist_removen

```c
int slist_removen(struct slist *slist, struct slist_node *node, void **dest);
```

Removes node after `node`, when `node` is NULL removes at head, if `dest` is not NULL stores removed data pointer into `*dest`, if `dest` is NULL and destroy callback exists then callback is called for removed data, returns 0 on success, -1 on error.

**Parameters**

- `slist` - pointer to list struct
- `node` - node before the node to remove, or NULL for head removal
- `dest` - output location for removed data pointer, or NULL

---

### slist_clear

```c
void slist_clear(struct slist *slist);
```

Removes all nodes, calls destroy callback for each node data when callback exists, no op when `slist` is NULL.

**Parameters**

- `slist` - pointer to list struct

---

## Example

```c
#include <slist.h>

int main(void)
{
  struct slist s;
  int a, b;
  void *out;

  if (slist_init(&s, NULL) != 0)
    return 1;

  a = 10;
  b = 20;
  slist_pushback(&s, &a);
  slist_pushfront(&s, &b); /* order is 20 then 10 */

  out = NULL;
  slist_popfront(&s, &out); /* out points to b */

  slist_clear(&s);
  slist_fini(&s);
  return 0;
}
```
