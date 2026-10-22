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

#ifndef COLLECTION_QUEUE_H
#define COLLECTION_QUEUE_H

#include <stddef.h>

struct queue;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Allocate and initialize an empty queue. Each stored element is a copy of the
 * value passed to enqueue operations.
 *
 * @param queue   output pointer to receive the new queue, must not be NULL
 * @param elesz   byte size of each element, must be non zero
 * @param destroy optional callback invoked on each element when it is
 *                discarded, or NULL for no op
 * @return        0 on success, -1 on failure
 */
extern int queue_init(struct queue **queue, size_t elesz,
                      void (*destroy)(void *));

/**
 * Destroy all elements, release the internal buffer, and free the queue.
 *
 * @param queue the queue to free, or NULL
 */
extern void queue_free(struct queue *queue);

/**
 * Test whether the queue contains no elements.
 *
 * @param queue the queue to inspect, or NULL
 * @return      non zero if empty, zero otherwise
 */
extern int queue_empty(const struct queue *queue);

/**
 * Return the current number of elements in the queue.
 *
 * @param queue the queue to inspect, or NULL
 * @return      element count, or 0 if queue is NULL
 */
extern size_t queue_size(const struct queue *queue);

/**
 * Return a pointer to the front element without removing it.
 *
 * @param queue the queue to inspect, or NULL
 * @return      pointer to the front element, or NULL if queue is NULL or empty
 */
extern void *queue_peek(const struct queue *queue);

/**
 * Append a copy of ele to the back of the queue.
 *
 * @param queue the queue to modify
 * @param ele   pointer to the value to copy, must not be NULL
 * @return      0 on success, -1 on failure
 */
extern int queue_enq(struct queue *queue, void *ele);

/**
 * Remove the front element. When dest is non NULL, copy the element there and
 * skip destroy, otherwise invoke destroy when set.
 *
 * @param queue the queue to modify
 * @param dest  buffer of at least elesz bytes to receive the element, or NULL
 *              to invoke destroy
 * @return      0 on success, -1 if queue is NULL or the queue is empty
 */
extern int queue_deq(struct queue *queue, void *dest);

/**
 * Remove all elements and reset size to zero, invoking destroy on each when
 * set. Does not release the buffer or free the queue.
 *
 * @param queue the queue to clear, or NULL
 */
extern void queue_clear(struct queue *queue);

#ifdef __cplusplus
}
#endif

#endif
