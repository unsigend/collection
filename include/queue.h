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

#ifndef _QUEUE_H
#define _QUEUE_H

#include <deque.h>
#include <stddef.h>

/* First-in, first-out (FIFO) queue. Based on deque. */

struct queue {
    struct deque deque;
};

#define queue_len(q) ((q)->deque.len)
#define queue_elesz(q) ((q)->deque.elesz)
#define queue_empty(q) ((q)->deque.len == 0)
#define queue_full(q) ((q)->deque.len == (q)->deque.cap)
#define queue_avail(q) ((q)->deque.cap - (q)->deque.len)

extern int queue_init(struct queue *q, size_t elesz, size_t cap);
extern int queue_init_empty(struct queue *q, size_t elesz);

extern void queue_free(struct queue *q);
extern void queue_clear(struct queue *q);

extern void *queue_back(struct queue *q);
extern void *queue_front(struct queue *q);

extern int queue_push(struct queue *q, const void *ele);
extern int queue_pop(struct queue *q, void *ele);

#endif
