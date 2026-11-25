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

#ifndef _DICT_H
#define _DICT_H

#include <stdbool.h>
#include <stddef.h>

/* Chained hash table. */

struct dict_pair {
    void *key;
    void *val;
};

struct dict_node {
    struct dict_pair pair;
    struct dict_node *next;
};

struct dict_fns {
    size_t (*hash)(const void *key);
    void *(*keydup)(const void *key);
    void *(*valdup)(const void *val);
    int (*keycmp)(const void *key1, const void *key2);
    void (*keyfree)(void *key);
    void (*valfree)(void *val);
};

struct dict {
    struct dict_node **buckets;
    size_t len;
    size_t cap;
    struct dict_fns fns;
};

struct dict_iter {
    struct dict *dict;
    size_t index;
    struct dict_node *node;
};

#define dict_len(d) ((d)->len)
#define dict_empty(d) ((d)->len == 0)
#define dict_buckets(d) ((d)->buckets)
#define dict_fns(d) ((d)->fns)
#define dict_pair(n) ((n)->pair)
#define dict_key(n) ((n)->pair.key)
#define dict_val(n) ((n)->pair.val)

#define dict_sethash(d, fn) ((d)->fns.hash = (fn))
#define dict_setkeydup(d, fn) ((d)->fns.keydup = (fn))
#define dict_setvaldup(d, fn) ((d)->fns.valdup = (fn))
#define dict_setkeycmp(d, fn) ((d)->fns.keycmp = (fn))
#define dict_setkeyfree(d, fn) ((d)->fns.keyfree = (fn))
#define dict_setvalfree(d, fn) ((d)->fns.valfree = (fn))

#define dict_gethash(d) ((d)->fns.hash)
#define dict_getkeydup(d) ((d)->fns.keydup)
#define dict_getvaldup(d) ((d)->fns.valdup)
#define dict_getkeycmp(d) ((d)->fns.keycmp)
#define dict_getkeyfree(d) ((d)->fns.keyfree)
#define dict_getvalfree(d) ((d)->fns.valfree)

extern int dict_init(struct dict *d, struct dict_fns *fns);
extern void dict_free(struct dict *d);
extern void dict_clear(struct dict *d);

extern bool dict_has(struct dict *d, void *key);
extern int dict_get(struct dict *d, void *key, void **val);
extern int dict_getn(struct dict *d, void *key, struct dict_node **node);
extern int dict_set(struct dict *d, void *key, void *val);
extern int dict_sets(struct dict *d, void *key,
                     void *val); /* Insert if not exists */
extern int dict_del(struct dict *d, void *key, struct dict_pair *pair);
extern int dict_deln(struct dict *d, struct dict_node *node,
                     struct dict_pair *pair);
extern int dict_dup(struct dict *d, struct dict *dup);

extern void dict_iter_init(struct dict_iter *iter, struct dict *dict);
extern void dict_iter_next(struct dict_iter *iter);
extern bool dict_iter_done(struct dict_iter *iter);
extern struct dict_node *dict_iter_node(struct dict_iter *iter);
extern struct dict_pair *dict_iter_get(struct dict_iter *iter);

#endif