/* collection - A high-performance C data structure library
 * Copyright (C) 2026-2027 Yixiang Qiu
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

#ifndef _DLIST_H
#define _DLIST_H

#include <stdbool.h>
#include <stddef.h>

/* Generic doubly linked list implementation. */

struct dlist_node {
    struct dlist_node *prev;
    struct dlist_node *next;
    void *val;
};

struct dlist {
    struct dlist_node *head;
    struct dlist_node *tail;
    size_t len;
    void *(*dup)(void *p);
    void (*free)(void *p);
    bool (*match)(void *p, void *key);
};

#define dlist_len(l) ((l)->len)
#define dlist_empty(l) ((l)->len == 0)
#define dlist_front(l) ((l)->head)
#define dlist_back(l) ((l)->tail)
#define dlist_prevnode(n) ((n)->prev)
#define dlist_nextnode(n) ((n)->next)
#define dlist_nodeval(n) ((n)->val)

#define dlist_setdup(l, fn) ((l)->dup = (fn))
#define dlist_setfree(l, fn) ((l)->free = (fn))
#define dlist_setmatch(l, fn) ((l)->match = (fn))

#define dlist_getdup(l) ((l)->dup)
#define dlist_getfree(l) ((l)->free)
#define dlist_getmatch(l) ((l)->match)

extern void dlist_init(struct dlist *l);
extern void dlist_free(struct dlist *l);
extern void dlist_clear(struct dlist *l);

extern int dlist_add_head(struct dlist *l, void *val);
extern int dlist_add_tail(struct dlist *l, void *val);
extern int dlist_del_head(struct dlist *l, void **val);
extern int dlist_del_tail(struct dlist *l, void **val);
extern int dlist_ins(struct dlist *l, struct dlist_node *where, void *val,
                     int after);
extern int dlist_del(struct dlist *l, struct dlist_node *node, void **val);

extern int dlist_dup(struct dlist *l, struct dlist *dup);
extern struct dlist_node *dlist_search(struct dlist *l, void *key);
extern struct dlist_node *dlist_at(struct dlist *l, size_t index);

#endif