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

#ifndef _DEQUE_H
#define _DEQUE_H

#include <stddef.h>

/* Double-ended queue. Slab-based. Store scalar elements of a fixed elesz. */

struct deque {
    void *buf;
    size_t len;
    size_t cap;
    size_t elesz;
    size_t head;
};

#define deque_len(d) ((d)->len)
#define deque_cap(d) ((d)->cap)
#define deque_elesz(d) ((d)->elesz)
#define deque_empty(d) ((d)->len == 0)
#define deque_full(d) ((d)->len == (d)->cap)
#define deque_avail(d) ((d)->cap - (d)->len)
#define deque_head(d) ((d)->head)

extern int deque_init(struct deque *d, size_t elesz, size_t cap);
extern int deque_init_empty(struct deque *d, size_t elesz);

extern void deque_free(struct deque *d);
extern void deque_clear(struct deque *d);

extern void *deque_at(struct deque *d, size_t index);
extern void *deque_front(struct deque *d);
extern void *deque_back(struct deque *d);

extern int deque_reserve(struct deque *d, size_t cap);
extern void deque_shrink(struct deque *d);

extern int deque_pushf(struct deque *d, const void *ele);
extern int deque_pushb(struct deque *d, const void *ele);
extern int deque_popf(struct deque *d, void *ele);
extern int deque_popb(struct deque *d, void *ele);
extern int deque_ins(struct deque *d, size_t index, const void *ele);
extern int deque_del(struct deque *d, size_t index, void *ele);

extern int deque_dup(struct deque *d, struct deque *dup);
extern void deque_sort(struct deque *d, int (*cmp)(const void *, const void *));

#endif /* _DEQUE_H */