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

#include <dlist.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

void dlist_init(struct dlist *l) { memset(l, 0, sizeof(*l)); }

void dlist_free(struct dlist *l)
{
    if (l == NULL)
        return;

    dlist_clear(l);
    memset(l, 0, sizeof(*l));
}

void dlist_clear(struct dlist *l)
{
    struct dlist_node *node, *next;

    node = l->head;
    while (node) {
        next = node->next;
        if (l->free)
            l->free(node->val);
        free(node);
        node = next;
    }
    l->head = l->tail = NULL;
    l->len = 0;
}

int dlist_add_head(struct dlist *l, void *val)
{
    struct dlist_node *node;

    node = malloc(sizeof(struct dlist_node));
    if (node == NULL)
        return -ENOMEM;

    node->val = val;
    node->prev = NULL;
    node->next = l->head;
    if (l->head)
        l->head->prev = node;
    else
        l->tail = node;
    l->head = node;
    l->len++;

    return 0;
}

int dlist_add_tail(struct dlist *l, void *val)
{
    struct dlist_node *node;

    node = malloc(sizeof(struct dlist_node));
    if (node == NULL)
        return -ENOMEM;

    node->val = val;
    node->prev = l->tail;
    node->next = NULL;
    if (l->tail)
        l->tail->next = node;
    else
        l->head = node;
    l->tail = node;
    l->len++;

    return 0;
}

int dlist_ins(struct dlist *l, struct dlist_node *where, void *val, int after)
{
    struct dlist_node *node;

    node = malloc(sizeof(struct dlist_node));
    if (node == NULL)
        return -ENOMEM;

    node->val = val;

    if (after) {
        node->prev = where;
        node->next = where->next;
        if (where->next)
            where->next->prev = node;
        else
            l->tail = node;
        where->next = node;
    } else {
        node->prev = where->prev;
        node->next = where;
        if (where->prev)
            where->prev->next = node;
        else
            l->head = node;
        where->prev = node;
    }
    l->len++;
    return 0;
}

int dlist_del_head(struct dlist *l, void **val)
{
    struct dlist_node *next;

    if (l->len == 0)
        return -ENOENT;

    if (val)
        *val = l->head->val;
    else if (l->free)
        l->free(l->head->val);

    next = l->head->next;
    if (next)
        next->prev = NULL;
    else
        l->tail = NULL;
    free(l->head);
    l->head = next;
    l->len--;

    return 0;
}

int dlist_del_tail(struct dlist *l, void **val)
{
    struct dlist_node *prev;

    if (l->len == 0)
        return -ENOENT;

    if (val)
        *val = l->tail->val;
    else if (l->free)
        l->free(l->tail->val);

    prev = l->tail->prev;
    if (prev)
        prev->next = NULL;
    else
        l->head = NULL;
    free(l->tail);
    l->tail = prev;
    l->len--;

    return 0;
}

int dlist_del(struct dlist *l, struct dlist_node *node, void **val)
{
    struct dlist_node *prev, *next;

    if (val)
        *val = node->val;
    else if (l->free)
        l->free(node->val);

    prev = node->prev;
    next = node->next;
    if (prev)
        prev->next = next;
    else
        l->head = next;
    if (next)
        next->prev = prev;
    else
        l->tail = prev;
    free(node);
    l->len--;

    return 0;
}

int dlist_dup(struct dlist *l, struct dlist *dup)
{
    struct dlist_node *node;
    int err;

    if (l == dup)
        return -EINVAL;

    dlist_init(dup);

    dup->dup = l->dup;
    dup->free = l->free;
    dup->match = l->match;

    node = l->head;
    while (node) {
        void *val;

        if (l->dup) {
            val = l->dup(node->val);
            if (val == NULL) {
                dlist_clear(dup);
                return -ENOMEM;
            }
        } else
            val = node->val;

        if ((err = dlist_add_tail(dup, val))) {
            if (l->dup && l->free)
                l->free(val);
            dlist_clear(dup);
            return err;
        }
        node = node->next;
    }
    return 0;
}

struct dlist_node *dlist_search(struct dlist *l, void *key)
{
    struct dlist_node *node;

    node = l->head;
    while (node) {
        if (l->match && l->match(node->val, key))
            return node;
        else if (!l->match && node->val == key)
            return node;
        node = node->next;
    }
    return NULL;
}

struct dlist_node *dlist_at(struct dlist *l, size_t index)
{
    struct dlist_node *node;

    if (index >= l->len)
        return NULL;

    node = l->head;
    while (index--)
        node = node->next;
    return node;
}