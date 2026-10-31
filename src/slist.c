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

#include <slist.h>
#include <stdlib.h>
#include <string.h>

struct slist *slist_init(void)
{
    struct slist *l;

    l = (struct slist *)malloc(sizeof(struct slist));
    if (l == NULL)
        return NULL;

    memset(l, 0, sizeof(struct slist));

    return l;
}

void slist_free(struct slist *l)
{
    if (l == NULL)
        return;

    slist_clear(l);
    free(l);
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
    l->head = NULL;
    l->tail = NULL;
    l->len = 0;
}

struct slist *slist_add_head(struct slist *l, void *value)
{
    struct slist_node *node;

    node = (struct slist_node *)malloc(sizeof(struct slist_node));
    if (node == NULL)
        return NULL;

    node->val = value;
    node->next = l->head;
    l->head = node;

    if (l->tail == NULL)
        l->tail = node;

    l->len++;

    return l;
}

struct slist *slist_add_tail(struct slist *l, void *value)
{
    struct slist_node *node;

    node = (struct slist_node *)malloc(sizeof(struct slist_node));
    if (node == NULL)
        return NULL;

    node->val = value;
    node->next = NULL;
    if (l->tail)
        l->tail->next = node;
    l->tail = node;

    if (l->head == NULL)
        l->head = node;

    l->len++;

    return l;
}

struct slist *slist_insert(struct slist *l, struct slist_node *where,
                           void *value)
{
    struct slist_node *node;

    node = (struct slist_node *)malloc(sizeof(struct slist_node));
    if (node == NULL)
        return NULL;

    node->val = value;
    node->next = where->next;
    where->next = node;

    if (where == l->tail)
        l->tail = node;

    l->len++;

    return l;
}

void slist_del(struct slist *l, struct slist_node *node)
{
    struct slist_node *cur = NULL;
    struct slist_node *next = l->head;

    while (next) {
        if (next == node) {
            if (cur == NULL)
                l->head = next->next;
            else
                cur->next = next->next;
            if (l->tail == next)
                l->tail = cur;
            if (l->free)
                l->free(next->val);
            free(next);
            l->len--;
            break;
        }
        cur = next;
        next = next->next;
    }
}

struct slist *slist_dup(struct slist *l)
{
    struct slist *newl;
    struct slist_node *node;

    newl = slist_init();
    if (newl == NULL)
        return NULL;

    newl->dup = l->dup;
    newl->free = l->free;
    newl->match = l->match;

    node = l->head;
    while (node) {
        void *value;

        if (newl->dup) {
            value = newl->dup(node->val);
            if (value == NULL) {
                slist_free(newl);
                return NULL;
            }
        } else
            value = node->val;

        if (slist_add_tail(newl, value) == NULL) {
            if (newl->dup && newl->free) {
                newl->free(value);
                slist_free(newl);
                return NULL;
            } else {
                newl->free = NULL;
                slist_free(newl);
                return NULL;
            }
        }
        node = node->next;
    }

    return newl;
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

struct slist_node *slist_index(struct slist *l, size_t index)
{
    struct slist_node *node;

    node = l->head;
    while (node && index--)
        node = node->next;
    return node;
}