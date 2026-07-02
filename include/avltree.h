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

#include <stddef.h>

struct avltree;
struct avltree_node;

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*avltree_fns_cmp)(void *, void *);
typedef void (*avltree_fns_destroy_key)(void *);
typedef void (*avltree_fns_destroy_val)(void *);

/**
 * Allocate and initialize an empty tree ordered by cmp.
 *
 * Keys and values are stored as opaque pointers; the tree does not copy
 * pointed-to data. destroy_key and destroy_val are invoked when a stored
 * pointer is discarded, or NULL for no-op.
 *
 * @param tree        output pointer to receive the new tree, must not be NULL
 * @param cmp         key comparator, must not be NULL
 * @param destroy_key optional callback invoked on each discarded key, or NULL
 * @param destroy_val optional callback invoked on each discarded value, or NULL
 * @return            0 on success, -1 on failure
 */
int avltree_init(struct avltree **tree, avltree_fns_cmp cmp,
                 avltree_fns_destroy_key destroy_key,
                 avltree_fns_destroy_val destroy_val);

/**
 * Destroy all entries and free the tree.
 *
 * @param tree the tree to free, or NULL
 */
void avltree_free(struct avltree *tree);

/**
 * Test whether the tree contains no entries.
 *
 * @param tree the tree to inspect, or NULL
 * @return     non-zero if empty, zero otherwise
 */
int avltree_empty(const struct avltree *tree);

/**
 * Return the current number of key-value entries in the tree.
 *
 * @param tree the tree to inspect, or NULL
 * @return     entry count, or 0 if tree is NULL
 */
size_t avltree_size(const struct avltree *tree);

/**
 * Return the root node of the tree.
 *
 * @param tree the tree to inspect, or NULL
 * @return     root node, or NULL if tree is NULL or empty
 */
struct avltree_node *avltree_root(const struct avltree *tree);

/**
 * Look up the value associated with key.
 *
 * @param tree the tree to search, or NULL
 * @param key  key to find, must not be NULL
 * @return     stored value pointer, or NULL if tree is NULL, key is NULL, or
 *             the key is not present
 */
void *avltree_find(const struct avltree *tree, void *key);

/**
 * Look up the node associated with key.
 *
 * @param tree the tree to search, or NULL
 * @param key  key to find, must not be NULL
 * @return     matching node, or NULL if tree is NULL, key is NULL, or the key
 *             is not present
 */
struct avltree_node *avltree_findnode(const struct avltree *tree, void *key);

/**
 * Insert a key-value pair. Fails when key is already present.
 *
 * @param tree the tree to modify
 * @param key  key pointer to store, must not be NULL
 * @param val  value pointer to store, may be NULL
 * @return     0 on success, -1 on error or if key already exists
 */
int avltree_insert(struct avltree *tree, void *key, void *val);

/**
 * Replace the value for an existing key. When dest is non-NULL, store the
 * previous value pointer in *dest and skip destroy_val; otherwise invoke
 * destroy_val when set.
 *
 * @param tree   the tree to modify
 * @param key    key to update, must not be NULL
 * @param newval new value pointer, must not be NULL
 * @param dest   output location for the previous value pointer, or NULL to
 *               invoke destroy_val
 * @return       0 on success, -1 if tree is NULL, arguments are invalid, or
 *               key is not present
 */
int avltree_update(struct avltree *tree, void *key, void *newval, void **dest);

/**
 * Remove the entry for key. When dest is non-NULL, store the removed value
 * pointer in *dest and skip destroy_val; otherwise invoke destroy_val when
 * set. destroy_key is always invoked on the removed key when set.
 *
 * @param tree the tree to modify
 * @param key  key to remove, must not be NULL
 * @param dest output location for the removed value pointer, or NULL to invoke
 *             destroy_val
 * @return     0 on success, -1 if tree is NULL, key is NULL, or key is not
 *             present
 */
int avltree_remove(struct avltree *tree, void *key, void **dest);

/**
 * Remove all entries and reset size to zero, invoking destroy_key and
 * destroy_val on each stored pointer when set. Does not free the tree itself.
 * No-op if tree is NULL.
 *
 * @param tree the tree to clear, or NULL
 */
void avltree_clear(struct avltree *tree);

#ifdef __cplusplus
}
#endif

#endif
