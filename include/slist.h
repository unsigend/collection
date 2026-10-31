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

#ifndef _SLIST_H_
#define _SLIST_H_

/* A simple generic singly linked list implementation. */

#include <stddef.h>

struct slist_node {
    struct slist_node *next;
    void *val;
};

struct slist {
    struct slist_node *head;
    struct slist_node *tail;
    size_t len;
    void *(*dup)(void *p);
    void (*free)(void *p);
    int (*match)(void *p, void *key);
};

#define slist_len(l) ((l)->len)
#define slist_first(l) ((l)->head)
#define slist_last(l) ((l)->tail)
#define slist_empty(l) ((l)->len == 0)
#define slist_nextnode(n) ((n)->next)
#define slist_val(n) ((n)->val)

#define slist_setdup(l, d) ((l)->dup = (d))
#define slist_setfree(l, f) ((l)->free = (f))
#define slist_setmatch(l, m) ((l)->match = (m))
#define slist_getdup(l) ((l)->dup)
#define slist_getfree(l) ((l)->free)
#define slist_getmatch(l) ((l)->match)

#ifdef __cplusplus
extern "C" {
#endif

extern struct slist *slist_init(void);
extern void slist_free(struct slist *l);

extern void slist_clear(struct slist *l);
extern struct slist *slist_add_head(struct slist *l, void *value);
extern struct slist *slist_add_tail(struct slist *l, void *value);
extern struct slist *slist_insert(struct slist *l, struct slist_node *where,
                                  void *value);
extern void slist_del(struct slist *l, struct slist_node *node);
extern struct slist *slist_dup(struct slist *l);
extern struct slist_node *slist_search(struct slist *l, void *key);
extern struct slist_node *slist_index(struct slist *l, size_t index);

#ifdef __cplusplus
}
#endif

#endif /* _SLIST_H_ */