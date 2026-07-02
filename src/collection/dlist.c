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

#include <dlist.h>
#include <stdlib.h>
#include <string.h>

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

struct dlist_iter {
    struct dlist *dlist;
    struct dlist_node *node;
};

static struct dlist_node *create_node(void *data)
{
    struct dlist_node *node = malloc(sizeof(struct dlist_node));
    if (!node)
        return NULL;
    memset(node, 0, sizeof(struct dlist_node));
    node->data = data;
    return node;
}

int dlist_init(struct dlist **dlist, void (*destroy)(void *))
{
    if (!dlist)
        return -1;
    struct dlist *d = calloc(1, sizeof(struct dlist));
    if (!d)
        return -1;
    d->destroy = destroy;
    *dlist = d;
    return 0;
}

void dlist_free(struct dlist *dlist)
{
    if (!dlist)
        return;
    dlist_clear(dlist);
    free(dlist);
}

size_t dlist_size(const struct dlist *dlist)
{
    if (!dlist)
        return 0;
    return dlist->len;
}

int dlist_empty(const struct dlist *dlist)
{
    if (!dlist)
        return 1;
    return dlist->len == 0;
}

void *dlist_front(const struct dlist *dlist)
{
    if (!dlist)
        return NULL;
    return dlist->head ? dlist->head->data : NULL;
}

void *dlist_back(const struct dlist *dlist)
{
    if (!dlist)
        return NULL;
    return dlist->tail ? dlist->tail->data : NULL;
}

struct dlist_node *dlist_head(const struct dlist *dlist)
{
    if (!dlist)
        return NULL;
    return dlist->head;
}

struct dlist_node *dlist_tail(const struct dlist *dlist)
{
    if (!dlist)
        return NULL;
    return dlist->tail;
}

struct dlist_node *dlist_next(const struct dlist_node *node)
{
    if (!node)
        return NULL;
    return node->next;
}

struct dlist_node *dlist_prev(const struct dlist_node *node)
{
    if (!node)
        return NULL;
    return node->prev;
}

void *dlist_data(const struct dlist_node *node)
{
    if (!node)
        return NULL;
    return node->data;
}

void dlist_clear(struct dlist *dlist)
{
    if (!dlist)
        return;
    struct dlist_node *node = dlist->head;
    while (node) {
        struct dlist_node *next = dlist_next(node);
        if (dlist->destroy)
            dlist->destroy(node->data);
        free(node);
        node = next;
    }
    dlist->head = NULL;
    dlist->tail = NULL;
    dlist->len = 0;
}

int dlist_pushfront(struct dlist *dlist, void *data)
{
    if (!dlist || !data)
        return -1;
    struct dlist_node *node = create_node(data);
    if (!node)
        return -1;
    node->next = dlist->head;
    if (dlist->head)
        dlist->head->prev = node;
    else
        dlist->tail = node;
    dlist->head = node;
    dlist->len++;
    return 0;
}

int dlist_pushback(struct dlist *dlist, void *data)
{
    if (!dlist || !data)
        return -1;
    struct dlist_node *node = create_node(data);
    if (!node)
        return -1;
    node->prev = dlist->tail;
    if (dlist->tail)
        dlist->tail->next = node;
    else
        dlist->head = node;
    dlist->tail = node;
    dlist->len++;
    return 0;
}

int dlist_popfront(struct dlist *dlist, void **dest)
{
    if (!dlist || dlist_empty(dlist))
        return -1;
    if (dest)
        *dest = dlist_front(dlist);
    else if (dlist->destroy)
        dlist->destroy(dlist_front(dlist));
    struct dlist_node *node = dlist->head;
    dlist->head = node->next;
    if (!dlist->head)
        dlist->tail = NULL;
    else
        dlist->head->prev = NULL;
    free(node);
    dlist->len--;
    return 0;
}

int dlist_popback(struct dlist *dlist, void **dest)
{
    if (!dlist || dlist_empty(dlist))
        return -1;
    if (dest)
        *dest = dlist_back(dlist);
    else if (dlist->destroy)
        dlist->destroy(dlist_back(dlist));
    struct dlist_node *node = dlist->tail;
    dlist->tail = node->prev;
    if (!dlist->tail)
        dlist->head = NULL;
    else
        dlist->tail->next = NULL;
    free(node);
    dlist->len--;
    return 0;
}

int dlist_insert(struct dlist *dlist, struct dlist_node *node, void *data)
{
    if (!dlist || !data)
        return -1;
    if (!node)
        return dlist_pushfront(dlist, data);

    struct dlist_node *new_node = create_node(data);
    if (!new_node)
        return -1;
    new_node->next = node;
    new_node->prev = node->prev;
    if (node->prev)
        node->prev->next = new_node;
    else
        dlist->head = new_node;
    node->prev = new_node;
    dlist->len++;
    return 0;
}

int dlist_remove(struct dlist *dlist, struct dlist_node *node, void **dest)
{
    if (!dlist)
        return -1;
    if (!node)
        return dlist_popfront(dlist, dest);
    if (dest)
        *dest = node->data;
    else if (dlist->destroy)
        dlist->destroy(node->data);
    if (node->prev)
        node->prev->next = node->next;
    else
        dlist->head = node->next;
    if (node->next)
        node->next->prev = node->prev;
    else
        dlist->tail = node->prev;
    free(node);
    dlist->len--;
    return 0;
}

int dlist_iter_init(struct dlist_iter **iter, struct dlist *dlist)
{
    if (!iter || !dlist)
        return -1;
    struct dlist_iter *i = calloc(1, sizeof(struct dlist_iter));
    if (!i)
        return -1;
    i->dlist = dlist;
    i->node = dlist->head;
    *iter = i;
    return 0;
}

int dlist_iter_init_rev(struct dlist_iter **iter, struct dlist *dlist)
{
    if (!iter || !dlist)
        return -1;
    struct dlist_iter *i = calloc(1, sizeof(struct dlist_iter));
    if (!i)
        return -1;
    i->dlist = dlist;
    i->node = dlist->tail;
    *iter = i;
    return 0;
}

void dlist_iter_inc(struct dlist_iter *iter)
{
    if (!iter || !iter->node)
        return;
    iter->node = iter->node->next;
}

void dlist_iter_dec(struct dlist_iter *iter)
{
    if (!iter || !iter->node)
        return;
    iter->node = iter->node->prev;
}

void *dlist_iter_get(struct dlist_iter *iter)
{
    if (!iter)
        return NULL;
    return dlist_data(iter->node);
}

void dlist_iter_free(struct dlist_iter *iter)
{
    if (!iter)
        return;
    free(iter);
}