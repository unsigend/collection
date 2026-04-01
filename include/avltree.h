/* collection - A generic data structure and algorithms library
 * Copyright (C) 2025 Yixiang Qiu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef COL_AVLTREE_H
#define COL_AVLTREE_H

/* AVL tree implementation, for read heavy use cases only. */

#include <stddef.h>

struct avltree_node {
  void *key;
  void *val;
  int height;
  struct avltree_node *left;
  struct avltree_node *right;
};

struct avltree_fns {
  int (*cmp)(void *, void *);
  void (*destroy_key)(void *);
  void (*destroy_val)(void *);
};

struct avltree {
  struct avltree_node *root;
  size_t size;
  struct avltree_fns *fns;
};

#define avltree_empty(tree)                                                    \
  ((tree)->size == 0)                     /* Check if the rbtree is empty */
#define avltree_size(tree) ((tree)->size) /* Get the size of the avltree */
#define avltree_root(tree) ((tree)->root) /* Get the root of the avltree */
#define avltree_fns(tree) ((tree)->fns)   /* Get the fns of the avltree */

int avltree_init(struct avltree *tree, struct avltree_fns *fns);
void avltree_fini(struct avltree *tree);

/* Insert a new key-value pair into the rbtree. Returns 0 on success, -1 on
   error or if the key already exists */
int avltree_insert(struct avltree *tree, void *key, void *val);

/* Update the value of the given key. Returns 0 on success, -1 on error or if
   the key does not exist */
int avltree_update(struct avltree *tree, void *key, void *newval, void **dest);

/* Remove a key-value pair from the rbtree. Returns 0 on success, -1 on error
   or if the key does not exist */
int avltree_remove(struct avltree *tree, void *key, void **dest);

void *avltree_find(struct avltree *tree, void *key);
struct avltree_node *avltree_findnode(struct avltree *tree, void *key);

void avltree_clear(struct avltree *tree);

#endif