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

#define dlist_empty(dlist)                                                     \
  ((dlist)->len == 0)                    /* Check if the dlist is empty        \
                                          */
#define dlist_size(dlist) ((dlist)->len) /* Get the size of the dlist */
#define dlist_front(dlist)                                                     \
  ((dlist)->head ? (dlist)->head->data                                         \
                 : NULL) /* Get the front element of the dlist */
#define dlist_back(dlist)                                                      \
  ((dlist)->tail ? (dlist)->tail->data                                         \
                 : NULL) /* Get the back element of the dlist */
#define dlist_next(node)                                                       \
  ((node) ? (node)->next : NULL) /* Get the next node of the given node */
#define dlist_prev(node)                                                       \
  ((node) ? (node)->prev : NULL) /* Get the previous node of the given node */
#define dlist_data(node)                                                       \
  ((node) ? (node)->data : NULL) /* Get the data of the given node */

int dlist_init(struct dlist *dlist, void (*destroy)(void *));
void dlist_fini(struct dlist *dlist);

int dlist_pushfront(struct dlist *dlist, void *data);
int dlist_pushback(struct dlist *dlist, void *data);
int dlist_popfront(struct dlist *dlist, void **dest);
int dlist_popback(struct dlist *dlist, void **dest);

/* Insert before the given node, NULL to insert at head */
int dlist_insert(struct dlist *dlist, struct dlist_node *node, void *data);
/* Remove the given node, NULL to remove from head */
int dlist_remove(struct dlist *dlist, struct dlist_node *node, void **dest);

void dlist_clear(struct dlist *dlist);

struct dlist_iter {
  struct dlist *dlist;
  struct dlist_node *node;
};

int dlist_iter_init(struct dlist_iter *iter, struct dlist *dlist);
int dlist_iter_initrev(struct dlist_iter *iter,
                       struct dlist *dlist); /* reverse iterator */
void dlist_iter_inc(struct dlist_iter *iter);
void dlist_iter_dec(struct dlist_iter *iter);
void *dlist_iter_get(struct dlist_iter *iter);

#endif