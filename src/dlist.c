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
#include <stdlib.h>
#include <string.h>

struct dlist *dlist_init(void)
{
    struct dlist *l;

    l = (struct dlist *)malloc(sizeof(struct dlist));
    if (l == NULL)
        return NULL;

    memset(l, 0, sizeof(struct dlist));

    return l;
}

void dlist_free(struct dlist *l)
{
    if (l == NULL)
        return;

    dlist_clear(l);
    free(l);
}

void dlist_clear(struct dlist *l)
{
    struct dlist_node *node, *next;

    if (l == NULL)
        return;

    node = l->head;
    while (node) {
        next = node->next;
        if (l->free)
            l->free(node->value);
        free(node);
        node = next;
    }

    l->head = l->tail = NULL;
    l->len = 0;
}

struct dlist *dlist_add_head(struct dlist *l, void *value)
{
    struct dlist_node *node;

    node = (struct dlist_node *)malloc(sizeof(struct dlist_node));
    if (node == NULL)
        return NULL;

    node->value = value;
    node->prev = NULL;
    node->next = l->head;

    if (l->len == 0)
        l->head = l->tail = node;
    else {
        l->head->prev = node;
        l->head = node;
    }

    l->len++;
    return l;
}

struct dlist *dlist_add_tail(struct dlist *l, void *value)
{
    struct dlist_node *node;

    node = (struct dlist_node *)malloc(sizeof(struct dlist_node));
    if (node == NULL)
        return NULL;

    node->value = value;
    node->prev = l->tail;
    node->next = NULL;

    if (l->len == 0)
        l->head = l->tail = node;
    else {
        l->tail->next = node;
        l->tail = node;
    }

    l->len++;
    return l;
}

struct dlist *dlist_insert(struct dlist *l, struct dlist_node *where,
                           void *value, int after)
{
    struct dlist_node *node;

    node = (struct dlist_node *)malloc(sizeof(struct dlist_node));
    if (node == NULL)
        return NULL;

    node->value = value;
    if (after) {
        node->prev = where;
        node->next = where->next;
        if (where->next)
            where->next->prev = node;
        where->next = node;
        if (where == l->tail)
            l->tail = node;
    } else {
        node->prev = where->prev;
        node->next = where;
        if (where->prev)
            where->prev->next = node;
        where->prev = node;
        if (where == l->head)
            l->head = node;
    }

    if (l->len == 0)
        l->head = l->tail = node;

    l->len++;
    return l;
}

void dlist_del(struct dlist *l, struct dlist_node *node)
{
    if (node->prev)
        node->prev->next = node->next;
    if (node->next)
        node->next->prev = node->prev;
    if (l->head == node)
        l->head = node->next;
    if (l->tail == node)
        l->tail = node->prev;
    if (l->free)
        l->free(node->value);
    free(node);
    l->len--;
}

struct dlist *dlist_dup(struct dlist *l)
{
    struct dlist *newl;
    struct dlist_node *node;

    newl = dlist_init();
    if (newl == NULL)
        return NULL;

    newl->dup = l->dup;
    newl->free = l->free;
    newl->match = l->match;

    node = l->head;
    while (node) {
        void *value;

        if (newl->dup) {
            value = newl->dup(node->value);
            if (value == NULL) {
                dlist_free(newl);
                return NULL;
            }
        } else
            value = node->value;

        if (dlist_add_tail(newl, value) == NULL) {
            if (newl->dup && newl->free) {
                newl->free(value);
                dlist_free(newl);
                return NULL;
            } else {
                newl->free = NULL;
                dlist_free(newl);
                return NULL;
            }
        }
        node = node->next;
    }
    return newl;
}

struct dlist_node *dlist_search(struct dlist *l, void *key)
{
    struct dlist_node *node;

    node = l->head;
    while (node) {
        if (l->match && l->match(node->value, key))
            return node;
        if (!l->match && node->value == key)
            return node;
        node = node->next;
    }
    return NULL;
}

struct dlist_node *dlist_index(struct dlist *l, size_t index)
{
    struct dlist_node *node;

    if (index >= l->len)
        return NULL;
    node = l->head;
    while (index--)
        node = node->next;
    return node;
}