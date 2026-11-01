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

#ifndef _SLIST_H
#define _SLIST_H

#include <stdbool.h>
#include <stddef.h>

/* Simple singly linked list implementation. */

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
    bool (*match)(void *p, void *key);
};

#define slist_len(l) ((l)->len)
#define slist_empty(l) ((l)->len == 0)
#define slist_front(l) ((l)->head)
#define slist_back(l) ((l)->tail)
#define slist_nextnode(n) ((n)->next)
#define slist_nodeval(n) ((n)->val)

#define slist_setdup(l, fn) ((l)->dup = (fn))
#define slist_setfree(l, fn) ((l)->free = (fn))
#define slist_setmatch(l, fn) ((l)->match = (fn))

#define slist_getdup(l) ((l)->dup)
#define slist_getfree(l) ((l)->free)
#define slist_getmatch(l) ((l)->match)

extern void slist_init(struct slist *l);
extern void slist_free(struct slist *l);
extern void slist_clear(struct slist *l);

extern int slist_add_head(struct slist *l, void *val);
extern int slist_add_tail(struct slist *l, void *val);
extern int slist_del_head(struct slist *l, void **val);
extern int slist_insn(struct slist *l, struct slist_node *where, void *val);
extern int slist_deln(struct slist *l, struct slist_node *node, void **val);
extern int slist_sdel(struct slist *l, const void *s,
                      void **val); /* search and delete */

extern int slist_dup(struct slist *l, struct slist *dup);
extern struct slist_node *slist_search(struct slist *l, void *key);
extern struct slist_node *slist_at(struct slist *l, size_t index);

#endif /* _SLIST_H */
