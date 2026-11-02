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

#include <queue.h>

int queue_init(struct queue *q, size_t elesz, size_t cap)
{
    return deque_init(&q->deque, elesz, cap);
}

int queue_init_empty(struct queue *q, size_t elesz)
{
    return deque_init_empty(&q->deque, elesz);
}

void queue_free(struct queue *q)
{
    if (q == NULL)
        return;
    deque_free(&q->deque);
}

void queue_clear(struct queue *q) { deque_clear(&q->deque); }

void *queue_back(struct queue *q) { return deque_back(&q->deque); }
void *queue_front(struct queue *q) { return deque_front(&q->deque); }

int queue_push(struct queue *q, const void *ele)
{
    return deque_pushb(&q->deque, ele);
}

int queue_pop(struct queue *q, void *ele) { return deque_popf(&q->deque, ele); }