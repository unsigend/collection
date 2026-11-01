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

#include <errno.h>
#include <slist.h>
#include <stdlib.h>
#include <string.h>

void slist_init(struct slist *l) { memset(l, 0, sizeof(*l)); }

void slist_free(struct slist *l)
{
    if (l == NULL)
        return;

    slist_clear(l);
    memset(l, 0, sizeof(*l));
}

void slist_clear(struct slist *l)
{
    struct slist_node *node, *next;

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

int slist_add_head(struct slist *l, void *val)
{
    struct slist_node *node;

    node = malloc(sizeof(struct slist_node));
    if (node == NULL)
        return -ENOMEM;

    node->val = val;
    node->next = l->head;
    l->head = node;
    if (l->tail == NULL)
        l->tail = node;
    l->len++;

    return 0;
}

int slist_add_tail(struct slist *l, void *val)
{
    struct slist_node *node;

    node = malloc(sizeof(struct slist_node));
    if (node == NULL)
        return -ENOMEM;

    node->val = val;
    node->next = NULL;
    if (l->tail)
        l->tail->next = node;
    else
        l->head = node;
    l->tail = node;
    l->len++;

    return 0;
}

int slist_del_head(struct slist *l, void **val)
{
    struct slist_node *next;

    if (l->len == 0)
        return -ENOENT;

    if (val)
        *val = l->head->val;
    else if (l->free)
        l->free(l->head->val);

    next = l->head->next;
    free(l->head);
    l->head = next;
    if (l->head == NULL)
        l->tail = NULL;
    l->len--;

    return 0;
}

int slist_insn(struct slist *l, struct slist_node *where, void *val)
{
    struct slist_node *node;

    node = malloc(sizeof(struct slist_node));
    if (node == NULL)
        return -ENOMEM;

    node->val = val;
    node->next = where->next;
    where->next = node;
    if (l->tail == where)
        l->tail = node;
    l->len++;

    return 0;
}

int slist_deln(struct slist *l, struct slist_node *node, void **val)
{
    struct slist_node *next;

    if (l->len == 0 || node->next == NULL)
        return -ENOENT;

    if (val)
        *val = node->next->val;
    else if (l->free)
        l->free(node->next->val);

    next = node->next;
    node->next = next->next;
    if (l->tail == next)
        l->tail = node;
    free(next);
    l->len--;

    return 0;
}

int slist_sdel(struct slist *l, const void *s, void **val)
{
    struct slist_node *node, *next;

    if (l->len == 0)
        return -ENOENT;

    node = l->head;

    if (l->match && l->match(node->val, (void *)s)) {
        slist_del_head(l, val);
        return 0;
    } else if (!l->match && node->val == s) {
        slist_del_head(l, val);
        return 0;
    }

    next = node->next;
    while (node && next) {
        if (l->match && l->match(next->val, (void *)s)) {
            slist_deln(l, node, val);
            return 0;
        } else if (!l->match && next->val == s) {
            slist_deln(l, node, val);
            return 0;
        }
        node = next;
        next = next->next;
    }

    return -ENOENT;
}

int slist_dup(struct slist *l, struct slist *dup)
{
    struct slist_node *node;
    int err;

    if (l == dup)
        return -EINVAL;

    slist_init(dup);
    dup->dup = l->dup;
    dup->free = l->free;
    dup->match = l->match;

    node = l->head;
    while (node) {
        void *val;

        if (l->dup) {
            val = l->dup(node->val);
            if (val == NULL) {
                slist_clear(dup);
                return -ENOMEM;
            }
        } else
            val = node->val;

        if ((err = slist_add_tail(dup, val))) {
            if (l->dup && l->free)
                l->free(val);
            slist_clear(dup);
            return err;
        }
        node = node->next;
    }
    return 0;
}

struct slist_node *slist_search(struct slist *l, void *key)
{
    struct slist_node *node;

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

struct slist_node *slist_at(struct slist *l, size_t index)
{
    struct slist_node *node;

    if (index >= l->len)
        return NULL;

    node = l->head;
    while (index--)
        node = node->next;
    return node;
}