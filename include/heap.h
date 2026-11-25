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

#ifndef _HEAP_H
#define _HEAP_H

/* Binary heap: a complete binary tree stored in a dynamic array. */

#include <dvec.h>

struct heap {
    struct dvec dvec;
    int (*cmp)(const void *, const void *);
};

#define heap_len(h) (dvec_len(&(h)->dvec))
#define heap_empty(h) (dvec_empty(&(h)->dvec))
#define heap_avail(h) (dvec_avail(&(h)->dvec))

#define heap_cmp(h) ((h)->cmp)
#define heap_setcmp(h, cmp) ((h)->cmp = (cmp))

extern int heap_init(struct heap *h, size_t elesz, size_t cap,
                     int (*cmp)(const void *, const void *));
extern int heap_init_empty(struct heap *h, size_t elesz,
                           int (*cmp)(const void *, const void *));
extern void heap_free(struct heap *h);
extern void heap_clear(struct heap *h);

extern void *heap_top(struct heap *h);
extern int heap_heapify(struct heap *h, const void *base, size_t n);
extern int heap_push(struct heap *h, const void *ele);
extern int heap_pop(struct heap *h, void *ele);
extern int heap_dup(struct heap *h, struct heap *dup);

#endif