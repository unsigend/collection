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

#ifndef _DLIST_H_
#define _DLIST_H_

/* A simple generic doubly linked list implementation. */

#include <stddef.h>

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
    int (*match)(void *p, void *key);
};

#define dlist_len(l) ((l)->len)
#define dlist_first(l) ((l)->head)
#define dlist_last(l) ((l)->tail)
#define dlist_empty(l) ((l)->len == 0)
#define dlist_prevnode(n) ((n)->prev)
#define dlist_nextnode(n) ((n)->next)
#define dlist_nodevalue(n) ((n)->val)

#define dlist_setdup(l, d) ((l)->dup = (d))
#define dlist_setfree(l, f) ((l)->free = (f))
#define dlist_setmatch(l, m) ((l)->match = (m))
#define dlist_getdup(l) ((l)->dup)
#define dlist_getfree(l) ((l)->free)
#define dlist_getmatch(l) ((l)->match)

#ifdef __cplusplus
extern "C" {
#endif

extern struct dlist *dlist_init(void);
extern void dlist_free(struct dlist *l);

extern void dlist_clear(struct dlist *l);
extern struct dlist *dlist_add_head(struct dlist *l, void *value);
extern struct dlist *dlist_add_tail(struct dlist *l, void *value);
extern struct dlist *dlist_insert(struct dlist *l, struct dlist_node *where,
                                  void *value, int after);
extern void dlist_del(struct dlist *l, struct dlist_node *node);
extern struct dlist *dlist_dup(struct dlist *l);
extern struct dlist_node *dlist_search(struct dlist *l, void *key);
extern struct dlist_node *dlist_index(struct dlist *l, size_t index);

#ifdef __cplusplus
}
#endif

#endif /* _DLIST_H_ */