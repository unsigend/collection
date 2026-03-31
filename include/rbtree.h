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

#ifndef COL_RBTREE_H
#define COL_RBTREE_H

#include <stddef.h>

/* For now the implementation is a simple AVL tree, refactor to RB-tree later */

struct rbtree_node {
  void *key;
  void *val;
  int height;
  struct rbtree_node *left;
  struct rbtree_node *right;
};

struct rbtree_fns {
  int (*cmp)(void *, void *);
  void (*destroy_key)(void *);
  void (*destroy_val)(void *);
};

struct rbtree {
  struct rbtree_node *root;
  size_t size;
  struct rbtree_fns *fns;
};

#define rbtree_empty(tree)                                                     \
  ((tree)->size == 0)                    /* Check if the rbtree is empty */
#define rbtree_size(tree) ((tree)->size) /* Get the size of the rbtree */
#define rbtree_root(tree) ((tree)->root) /* Get the root of the rbtree */
#define rbtree_fns(tree) ((tree)->fns)   /* Get the fns of the rbtree */

int rbtree_init(struct rbtree *tree, struct rbtree_fns *fns);
void rbtree_fini(struct rbtree *tree);

/* Insert a new key-value pair into the rbtree. Returns 0 on success, -1 on
   error or if the key already exists */
int rbtree_insert(struct rbtree *tree, void *key, void *val);

/* Update the value of the given key. Returns 0 on success, -1 on error or if
   the key does not exist */
int rbtree_update(struct rbtree *tree, void *key, void *newval, void **dest);

/* Remove a key-value pair from the rbtree. Returns 0 on success, -1 on error
   or if the key does not exist */
int rbtree_remove(struct rbtree *tree, void *key, void **dest);

void *rbtree_find(struct rbtree *tree, void *key);
struct rbtree_node *rbtree_findnode(struct rbtree *tree, void *key);

void rbtree_clear(struct rbtree *tree);

#endif