/* collection - A generic data structure and algorithms library for modern C
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

/**
 * This file contains the interface for queue.
 */
#include <collection/common.h>
#include <collection/slist.h>
#include <stdbool.h>

// internal struct for Queue
typedef struct {
    SList slist;
} Queue;

/**
 * Check if the queue is empty.
 * @param queue The queue to check.
 * @return True if the queue is empty, false otherwise.
 * @complexity O(1)
 */
extern bool queue_empty(const Queue* queue);

/**
 * Get the size of the queue.
 * @param queue The queue to get the size of.
 * @return The size of the queue.
 * @complexity O(1)
 */
extern size_t queue_size(const Queue* queue);

/**
 * Initialize the queue.
 * @param queue The queue to initialize.
 * @param destroy The function to destroy the elements.
 * @complexity O(1)
 */
extern void queue_init(Queue* queue, void (*destroy)(void *));

/**
 * Destroy the queue.
 * @param queue The queue to destroy.
 * @complexity O(n)
 */
extern void queue_destroy(Queue* queue);

/**
 * Enqueue an element into the queue.
 * @param queue The queue to enqueue the element into.
 * @param data The element to enqueue.
 * @return 0 if successful, -1 if failed
 * @complexity O(1)
 */
extern int queue_enqueue(Queue* queue, void* data);

/**
 * Dequeue an element from the queue.
 * @param queue The queue to dequeue the element from.
 * @param data The pointer to store the dequeued element.
 * @return 0 if successful, -1 if failed
 * @complexity O(1)
 */
extern int queue_dequeue(Queue* queue, void** data);

/**
 * Peek at the front element of the queue.
 * @param queue The queue to peek at.
 * @return The front element of the queue, or NULL if the queue is empty.
 * @complexity O(1)
 */
extern void* queue_peek(Queue* queue);

/**
 * Clear the queue.
 * @param queue The queue to clear.
 * @complexity O(n)
 */
extern void queue_clear(Queue* queue);

#endif