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

#ifndef _DVEC_H
#define _DVEC_H

#include <stddef.h>

/* Simple slab vector. Store scalar elements of a fixed elesz. */

struct dvec {
    void *buf;
    size_t len;
    size_t cap;
    size_t elesz;
};

#define dvec_raw(v) ((v)->buf)
#define dvec_len(v) ((v)->len)
#define dvec_cap(v) ((v)->cap)
#define dvec_elesz(v) ((v)->elesz)
#define dvec_empty(v) ((v)->len == 0)
#define dvec_avail(v) ((v)->cap - (v)->len)

extern int dvec_init(struct dvec *v, size_t elesz, size_t cap);
extern int dvec_init_empty(struct dvec *v, size_t elesz);

extern void dvec_free(struct dvec *v);
extern void dvec_clear(struct dvec *v);

extern void *dvec_at(struct dvec *v, size_t index);
extern void *dvec_back(struct dvec *v);
extern void *dvec_front(struct dvec *v);

extern int dvec_reserve(struct dvec *v, size_t cap);
extern void dvec_shrink(struct dvec *v);

extern int dvec_pushb(struct dvec *v, const void *ele);
extern int dvec_pushf(struct dvec *v, const void *ele);
extern int dvec_popb(struct dvec *v, void *ele);
extern int dvec_popf(struct dvec *v, void *ele);
extern int dvec_ins(struct dvec *v, size_t index, const void *ele);
extern int dvec_del(struct dvec *v, size_t index, void *ele);

extern int dvec_dup(struct dvec *v, struct dvec *dup);
extern void dvec_sort(struct dvec *v, int (*cmp)(const void *, const void *));

#endif