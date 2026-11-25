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

#include <dict.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define INIT_CAP 16
#define GROW_MUL 2

#define GROW_NUM 2
#define GROW_DEN 3

#define SHRINK_NUM 1
#define SHRINK_DEN 4

static inline bool should_grow(struct dict *d)
{
    return d->len * GROW_DEN >= d->cap * GROW_NUM;
}

static inline bool should_shrink(struct dict *d)
{
    return d->cap > INIT_CAP && d->len * SHRINK_DEN <= d->cap * SHRINK_NUM;
}

static inline size_t bucket_index(size_t (*hash)(const void *key), size_t cap,
                                  const void *key)
{
    return hash(key) & (cap - 1);
}

static void free_node(struct dict *d, struct dict_node *node)
{
    if (d->fns.keyfree)
        d->fns.keyfree(dict_key(node));
    if (d->fns.valfree)
        d->fns.valfree(dict_val(node));
    free(node);
}

static int add_node(struct dict *d, void *key, void *val)
{
    size_t index = bucket_index(d->fns.hash, d->cap, key);
    struct dict_node *node = calloc(1, sizeof(struct dict_node));
    if (node == NULL)
        return -ENOMEM;
    dict_key(node) = key;
    dict_val(node) = val;
    node->next = d->buckets[index];
    d->buckets[index] = node;
    d->len++;
    return 0;
}

static int rehash(struct dict *d, size_t newcap)
{
    struct dict_node *node, *next;
    size_t index;
    struct dict_node **newbuckets = calloc(newcap, sizeof(struct dict_node *));
    if (newbuckets == NULL)
        return -ENOMEM;

    for (size_t i = 0; i < d->cap; i++) {
        node = d->buckets[i];
        while (node) {
            next = node->next;
            index = bucket_index(d->fns.hash, newcap, dict_key(node));
            node->next = newbuckets[index];
            newbuckets[index] = node;
            node = next;
        }
    }

    free(d->buckets);
    d->buckets = newbuckets;
    d->cap = newcap;

    return 0;
}

static struct dict_node *find_node(struct dict *d, const void *key)
{
    size_t index = bucket_index(d->fns.hash, d->cap, key);
    struct dict_node *node = d->buckets[index];
    while (node) {
        if (d->fns.keycmp(dict_key(node), key) == 0)
            return node;
        node = node->next;
    }

    return NULL;
}

static struct dict_node *unlink_node(struct dict *d, void *key)
{
    size_t index = bucket_index(d->fns.hash, d->cap, key);
    struct dict_node *prev = NULL;
    struct dict_node *curr = d->buckets[index];

    while (curr) {
        if (d->fns.keycmp(dict_key(curr), key) == 0) {
            if (prev)
                prev->next = curr->next;
            else
                d->buckets[index] = curr->next;
            d->len--;
            return curr;
        }
        prev = curr;
        curr = curr->next;
    }

    return NULL;
}

static struct dict_node *unlink_nodeptr(struct dict *d, struct dict_node *node)
{
    size_t index = bucket_index(d->fns.hash, d->cap, dict_key(node));
    struct dict_node *prev = NULL;
    struct dict_node *curr = d->buckets[index];

    while (curr) {
        if (curr == node) {
            if (prev)
                prev->next = curr->next;
            else
                d->buckets[index] = curr->next;
            d->len--;
            return curr;
        }
        prev = curr;
        curr = curr->next;
    }

    return NULL;
}

static int finish_remove(struct dict *d, struct dict_node *node,
                         struct dict_pair *pair)
{
    if (pair) {
        pair->key = dict_key(node);
        pair->val = dict_val(node);
        free(node);
    } else
        free_node(d, node);
    if (should_shrink(d)) /* best effort */
        rehash(d, d->cap / GROW_MUL);
    return 0;
}

static struct dict_node *clone_node(struct dict *d, struct dict_node *src)
{
    struct dict_node *new = calloc(1, sizeof(struct dict_node));
    if (new == NULL)
        return NULL;
    if (d->fns.keydup) {
        dict_key(new) = d->fns.keydup(dict_key(src));
        if (dict_key(new) == NULL) {
            free(new);
            return NULL;
        }
    } else
        dict_key(new) = dict_key(src);
    if (d->fns.valdup) {
        dict_val(new) = d->fns.valdup(dict_val(src));
        if (dict_val(new) == NULL) {
            if (d->fns.keydup)
                d->fns.keyfree(dict_key(new));
            free(new);
            return NULL;
        }
    } else
        dict_val(new) = dict_val(src);
    return new;
}

int dict_init(struct dict *d, struct dict_fns *fns)
{
    memset(d, 0, sizeof(*d));
    if (fns) {
        if (fns->hash)
            dict_sethash(d, fns->hash);
        if (fns->keydup)
            dict_setkeydup(d, fns->keydup);
        if (fns->valdup)
            dict_setvaldup(d, fns->valdup);
        if (fns->keycmp)
            dict_setkeycmp(d, fns->keycmp);
        if (fns->keyfree)
            dict_setkeyfree(d, fns->keyfree);
        if (fns->valfree)
            dict_setvalfree(d, fns->valfree);
    }

    d->buckets = calloc(INIT_CAP, sizeof(struct dict_node *));
    if (d->buckets == NULL)
        return -ENOMEM;
    d->cap = INIT_CAP;

    return 0;
}

void dict_free(struct dict *d)
{
    if (d == NULL)
        return;

    dict_clear(d);
    free(d->buckets);
    memset(d, 0, sizeof(*d));
}

void dict_clear(struct dict *d)
{
    struct dict_node *node, *next;

    for (size_t i = 0; i < d->cap; i++) {
        node = d->buckets[i];
        while (node) {
            next = node->next;
            free_node(d, node);
            node = next;
        }
        d->buckets[i] = NULL;
    }
    d->len = 0;
}

bool dict_has(struct dict *d, void *key) { return find_node(d, key) != NULL; }

int dict_get(struct dict *d, void *key, void **val)
{
    struct dict_node *node = find_node(d, key);
    if (node == NULL)
        return -ENOENT;
    *val = dict_val(node);
    return 0;
}

int dict_getn(struct dict *d, void *key, struct dict_node **node)
{
    *node = find_node(d, key);
    return *node == NULL ? -ENOENT : 0;
}

int dict_set(struct dict *d, void *key, void *val)
{
    struct dict_node *node = find_node(d, key);
    int err = 0;

    if (node) {
        if (d->fns.valfree && val != dict_val(node))
            d->fns.valfree(dict_val(node));
        dict_val(node) = val;
        return 0;
    }

    if (should_grow(d) && (err = rehash(d, d->cap * GROW_MUL)))
        return err;

    return add_node(d, key, val);
}

int dict_sets(struct dict *d, void *key, void *val)
{
    struct dict_node *node = find_node(d, key);
    int err = 0;

    if (node)
        return -EEXIST;
    if (should_grow(d) && (err = rehash(d, d->cap * GROW_MUL)))
        return err;
    return add_node(d, key, val);
}

int dict_del(struct dict *d, void *key, struct dict_pair *pair)
{
    struct dict_node *node = unlink_node(d, key);
    if (node == NULL)
        return -ENOENT;
    return finish_remove(d, node, pair);
}

int dict_deln(struct dict *d, struct dict_node *node, struct dict_pair *pair)
{
    if (unlink_nodeptr(d, node) == NULL)
        return -ENOENT;
    return finish_remove(d, node, pair);
}

int dict_dup(struct dict *d, struct dict *dup)
{
    if (d == dup)
        return -EINVAL;
    memset(dup, 0, sizeof(*dup));
    dup->fns = d->fns;
    dup->buckets = calloc(d->cap, sizeof(struct dict_node *));
    if (dup->buckets == NULL)
        return -ENOMEM;
    dup->cap = d->cap;
    dup->len = d->len;

    for (size_t i = 0; i < d->cap; i++) {
        struct dict_node *node = d->buckets[i];
        while (node) {
            struct dict_node *new = clone_node(d, node);
            if (new == NULL) {
                dict_free(dup);
                return -ENOMEM;
            }
            new->next = dup->buckets[i];
            dup->buckets[i] = new;
            node = node->next;
        }
    }
    return 0;
}

void dict_iter_init(struct dict_iter *iter, struct dict *dict)
{
    memset(iter, 0, sizeof(*iter));
    iter->dict = dict;

    /* find the first non-empty bucket */
    for (size_t i = 0; i < dict->cap; i++) {
        if (dict->buckets[i]) {
            iter->index = i;
            iter->node = dict->buckets[i];
            break;
        }
    }
}

void dict_iter_next(struct dict_iter *iter)
{
    if (dict_iter_done(iter))
        return;
    if (iter->node->next) {
        iter->node = iter->node->next;
    } else {
        for (size_t i = iter->index + 1; i < iter->dict->cap; i++) {
            if (iter->dict->buckets[i]) {
                iter->index = i;
                iter->node = iter->dict->buckets[i];
                return;
            }
        }
        iter->node = NULL;
    }
}

bool dict_iter_done(struct dict_iter *iter) { return iter->node == NULL; }

struct dict_node *dict_iter_node(struct dict_iter *iter) { return iter->node; }

struct dict_pair *dict_iter_get(struct dict_iter *iter)
{
    return iter->node ? &iter->node->pair : NULL;
}