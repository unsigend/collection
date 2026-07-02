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

#ifndef COL_DLIST_H
#define COL_DLIST_H

#include <stddef.h>

struct dlist;
struct dlist_node;
struct dlist_iter;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Allocate and initialize an empty list with an optional destructor invoked on
 * each stored data pointer when it is discarded.
 *
 * @param dlist   output pointer to receive the new list, must not be NULL
 * @param destroy optional callback invoked on each data pointer when it is
 *                discarded, or NULL for no-op
 * @return        0 on success, -1 on failure
 */
int dlist_init(struct dlist **dlist, void (*destroy)(void *));

/**
 * Destroy all elements, release every node, and free the list.
 *
 * @param dlist the list to free, or NULL
 */
void dlist_free(struct dlist *dlist);

/**
 * Test whether the list contains no elements.
 *
 * @param dlist the list to inspect, or NULL
 * @return      non-zero if empty, zero otherwise
 */
int dlist_empty(const struct dlist *dlist);

/**
 * Return the current number of elements in the list.
 *
 * @param dlist the list to inspect, or NULL
 * @return      element count, or 0 if dlist is NULL
 */
size_t dlist_size(const struct dlist *dlist);

/**
 * Return the data pointer stored in the first node.
 *
 * @param dlist the list to inspect, or NULL
 * @return      front data pointer, or NULL if dlist is NULL or empty
 */
void *dlist_front(const struct dlist *dlist);

/**
 * Return the data pointer stored in the last node.
 *
 * @param dlist the list to inspect, or NULL
 * @return      back data pointer, or NULL if dlist is NULL or empty
 */
void *dlist_back(const struct dlist *dlist);

/**
 * Return the first node in the list.
 *
 * @param dlist the list to inspect, or NULL
 * @return      head node, or NULL if dlist is NULL or empty
 */
struct dlist_node *dlist_head(const struct dlist *dlist);

/**
 * Return the last node in the list.
 *
 * @param dlist the list to inspect, or NULL
 * @return      tail node, or NULL if dlist is NULL or empty
 */
struct dlist_node *dlist_tail(const struct dlist *dlist);

/**
 * Return the next node after node.
 *
 * @param node the node to inspect, or NULL
 * @return     next node, or NULL if node is NULL or has no successor
 */
struct dlist_node *dlist_next(const struct dlist_node *node);

/**
 * Return the previous node before node.
 *
 * @param node the node to inspect, or NULL
 * @return     previous node, or NULL if node is NULL or has no predecessor
 */
struct dlist_node *dlist_prev(const struct dlist_node *node);

/**
 * Return the data pointer stored in node.
 *
 * @param node the node to inspect, or NULL
 * @return     data pointer, or NULL if node is NULL
 */
void *dlist_data(const struct dlist_node *node);

/**
 * Prepend a data pointer to the front of the list.
 *
 * @param dlist the list to modify
 * @param data  pointer to store, must not be NULL
 * @return      0 on success, -1 on failure
 */
int dlist_pushfront(struct dlist *dlist, void *data);

/**
 * Append a data pointer to the back of the list.
 *
 * @param dlist the list to modify
 * @param data  pointer to store, must not be NULL
 * @return      0 on success, -1 on failure
 */
int dlist_pushback(struct dlist *dlist, void *data);

/**
 * Remove the front node. When dest is non-NULL, store the data pointer in
 * *dest and skip destroy; otherwise invoke destroy when set.
 *
 * @param dlist the list to modify
 * @param dest  output location for the front data pointer, or NULL to invoke
 *              destroy
 * @return      0 on success, -1 if dlist is NULL or the list is empty
 */
int dlist_popfront(struct dlist *dlist, void **dest);

/**
 * Remove the back node. When dest is non-NULL, store the data pointer in
 * *dest and skip destroy; otherwise invoke destroy when set.
 *
 * @param dlist the list to modify
 * @param dest  output location for the back data pointer, or NULL to invoke
 *              destroy
 * @return      0 on success, -1 if dlist is NULL or the list is empty
 */
int dlist_popback(struct dlist *dlist, void **dest);

/**
 * Insert a new node carrying data immediately before node. When node is NULL,
 * behaves like dlist_pushfront.
 *
 * @param dlist the list to modify
 * @param node  node to insert before, or NULL to insert at the head
 * @param data  pointer to store, must not be NULL
 * @return      0 on success, -1 on failure
 */
int dlist_insert(struct dlist *dlist, struct dlist_node *node, void *data);

/**
 * Remove node from the list. When node is NULL, behaves like dlist_popfront.
 * When dest is non-NULL, store the removed data pointer in *dest and skip
 * destroy; otherwise invoke destroy when set.
 *
 * @param dlist the list to modify
 * @param node  node to remove, or NULL to remove the head
 * @param dest  output location for the removed data pointer, or NULL to invoke
 *              destroy
 * @return      0 on success, -1 on failure
 */
int dlist_remove(struct dlist *dlist, struct dlist_node *node, void **dest);

/**
 * Remove all nodes and reset size to zero, invoking destroy on each data
 * pointer when set. Does not free the list itself. No-op if dlist is NULL.
 *
 * @param dlist the list to clear, or NULL
 */
void dlist_clear(struct dlist *dlist);

/**
 * Allocate and initialize an iterator at the first node of dlist.
 *
 * @param iter  output pointer to receive the new iterator, must not be NULL
 * @param dlist the list to traverse, must not be NULL
 * @return      0 on success, -1 on failure
 */
int dlist_iter_init(struct dlist_iter **iter, struct dlist *dlist);

/**
 * Allocate and initialize an iterator at the last node of dlist.
 *
 * @param iter  output pointer to receive the new iterator, must not be NULL
 * @param dlist the list to traverse, must not be NULL
 * @return      0 on success, -1 on failure
 */
int dlist_iter_init_rev(struct dlist_iter **iter, struct dlist *dlist);

/**
 * Advance the iterator to the next node.
 *
 * @param iter the iterator to advance, or NULL
 */
void dlist_iter_inc(struct dlist_iter *iter);

/**
 * Move the iterator to the previous node.
 *
 * @param iter the iterator to move, or NULL
 */
void dlist_iter_dec(struct dlist_iter *iter);

/**
 * Return the data pointer at the current iterator position.
 *
 * @param iter the iterator to read from, or NULL
 * @return     data pointer, or NULL if iter is NULL or not on a node
 */
void *dlist_iter_get(struct dlist_iter *iter);

/**
 * Free the iterator.
 *
 * @param iter the iterator to free, or NULL
 */
void dlist_iter_free(struct dlist_iter *iter);

#ifdef __cplusplus
}
#endif

#endif
