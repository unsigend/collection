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

#include <rbtree.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

static inline int height(struct rbtree_node *node)
{
  return node ? node->height : -1;
}

static inline int balancefactor(struct rbtree_node *node)
{
  return height(node->left) - height(node->right);
}

static struct rbtree_node *create_node(void *key, void *val)
{
  struct rbtree_node *node = calloc(1, sizeof(struct rbtree_node));
  if (!node)
    return NULL;
  node->key = key;
  node->val = val;
  return node;
}

static void clear(struct rbtree_node *node, struct rbtree_fns *fns)
{
  if (!node)
    return;
  struct rbtree_node *left = node->left;
  struct rbtree_node *right = node->right;
  if (fns->destroy_key)
    fns->destroy_key(node->key);
  if (fns->destroy_val)
    fns->destroy_val(node->val);
  free(node);
  clear(left, fns);
  clear(right, fns);
}

static struct rbtree_node *rotate_left(struct rbtree_node *node)
{
  struct rbtree_node *top = node->right;
  struct rbtree_node *oldleft = top->left;

  top->left = node;
  node->right = oldleft;

  node->height = 1 + MAX(height(node->left), height(node->right));
  top->height = 1 + MAX(height(top->left), height(top->right));
  return top;
}

static struct rbtree_node *rotate_right(struct rbtree_node *node)
{
  struct rbtree_node *top = node->left;
  struct rbtree_node *oldright = top->right;

  top->right = node;
  node->left = oldright;

  node->height = 1 + MAX(height(node->left), height(node->right));
  top->height = 1 + MAX(height(top->left), height(top->right));
  return top;
}

static struct rbtree_node *rebalance(struct rbtree_node *node)
{
  if (!node)
    return NULL;
  node->height = 1 + MAX(height(node->left), height(node->right));
  int bf = balancefactor(node);

  if (bf > 1 && balancefactor(node->left) >= 0) /* LL */
    return rotate_right(node);
  if (bf > 1 && balancefactor(node->left) < 0) { /* LR */
    node->left = rotate_left(node->left);
    return rotate_right(node);
  }
  if (bf < -1 && balancefactor(node->right) <= 0) /* RR */
    return rotate_left(node);
  if (bf < -1 && balancefactor(node->right) > 0) { /* RL */
    node->right = rotate_right(node->right);
    return rotate_left(node);
  }
  return node;
}

/* Recursively insert a node into the tree, return the new root node after
   insertion. If the node is inserted, set insf to 1. */
static struct rbtree_node *insert_node(struct rbtree_node *node, void *key,
                                       void *val, struct rbtree_fns *fns,
                                       int *insf)
{
  if (!node) {
    struct rbtree_node *new_node = create_node(key, val);
    if (new_node)
      *insf = 1;
    return new_node;
  }
  int cmp = fns->cmp(key, node->key);
  if (cmp > 0)
    node->right = insert_node(node->right, key, val, fns, insf);
  else if (cmp < 0)
    node->left = insert_node(node->left, key, val, fns, insf);
  else
    return node;

  return rebalance(node);
}

/* Recursively remove a node from the tree, return the new root node after
   removal. If the node is removed, set remf to 1. If unlink is set, free node
   only without destroying key and value. */
static struct rbtree_node *remove_node(struct rbtree_node *node, void *key,
                                       void **dest, struct rbtree_fns *fns,
                                       int *remf, int unlink)
{
  if (!node)
    return NULL;
  int cmp = fns->cmp(key, node->key);
  if (cmp > 0)
    node->right = remove_node(node->right, key, dest, fns, remf, unlink);
  else if (cmp < 0)
    node->left = remove_node(node->left, key, dest, fns, remf, unlink);
  else {
    *remf = 1;
    if (!node->left && !node->right) {
      if (!unlink) {
        if (fns->destroy_key)
          fns->destroy_key(node->key);
        if (dest)
          *dest = node->val;
        else if (fns->destroy_val)
          fns->destroy_val(node->val);
      }
      free(node);
      return NULL;
    } else if (node->left && node->right) {
      if (!unlink) {
        if (fns->destroy_key)
          fns->destroy_key(node->key);
        if (dest)
          *dest = node->val;
        else if (fns->destroy_val)
          fns->destroy_val(node->val);
      }

      struct rbtree_node *succ = node->right;
      while (succ->left)
        succ = succ->left;
      node->key = succ->key;
      node->val = succ->val;
      /* unlink the successor node */
      node->right = remove_node(node->right, succ->key, NULL, fns, remf, 1);
    } else {
      struct rbtree_node *child = node->left ? node->left : node->right;
      if (!unlink) {
        if (fns->destroy_key)
          fns->destroy_key(node->key);
        if (dest)
          *dest = node->val;
        else if (fns->destroy_val)
          fns->destroy_val(node->val);
      }

      free(node);
      return child;
    }
  }
  return rebalance(node);
}

int rbtree_init(struct rbtree *tree, struct rbtree_fns *fns)
{
  if (!tree || !fns || !fns->cmp)
    return -1;
  memset(tree, 0, sizeof(struct rbtree));
  tree->fns = fns;
  return 0;
}

void rbtree_clear(struct rbtree *tree)
{
  if (!tree)
    return;
  struct rbtree_node *node = tree->root;
  clear(node, tree->fns);
  tree->root = NULL;
  tree->size = 0;
}

void rbtree_fini(struct rbtree *tree)
{
  if (!tree)
    return;
  rbtree_clear(tree);
}

int rbtree_update(struct rbtree *tree, void *key, void *newval, void **dest)
{
  if (!tree || !key || !newval)
    return -1;
  struct rbtree_node *node = rbtree_findnode(tree, key);
  if (!node)
    return -1;
  if (dest)
    *dest = node->val;
  else if (tree->fns->destroy_val)
    tree->fns->destroy_val(node->val);
  node->val = newval;
  return 0;
}

void *rbtree_find(struct rbtree *tree, void *key)
{
  if (!tree || !key)
    return NULL;
  struct rbtree_node *node = rbtree_findnode(tree, key);
  return node ? node->val : NULL;
}

struct rbtree_node *rbtree_findnode(struct rbtree *tree, void *key)
{
  if (!tree || !key)
    return NULL;
  struct rbtree_node *node = tree->root;
  while (node) {
    int cmp = tree->fns->cmp(key, node->key);
    if (cmp == 0)
      return node;
    else if (cmp > 0)
      node = node->right;
    else
      node = node->left;
  }
  return NULL;
}

int rbtree_remove(struct rbtree *tree, void *key, void **dest)
{
  if (!tree || !key)
    return -1;
  int remf = 0;
  tree->root = remove_node(tree->root, key, dest, tree->fns, &remf, 0);
  if (remf)
    tree->size--;
  return remf ? 0 : -1;
}

int rbtree_insert(struct rbtree *tree, void *key, void *val)
{
  if (!tree || !key)
    return -1;
  int insf = 0;
  tree->root = insert_node(tree->root, key, val, tree->fns, &insf);
  if (insf)
    tree->size++;
  return insf ? 0 : -1;
}