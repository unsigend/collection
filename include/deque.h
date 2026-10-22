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

#ifndef COLLECTION_DEQUE_H
#define COLLECTION_DEQUE_H

#include <stddef.h>

struct deque;
struct deque_iter;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Allocate and initialize an empty deque with element size elesz and an
 * optional destructor. Each stored element is a copy of the value passed to
 * insert or push operations.
 *
 * @param deq     output pointer to receive the new deque, must not be NULL
 * @param elesz   byte size of each element, must be non zero
 * @param destroy optional callback invoked on each element when it is
 *                discarded, or NULL for no op
 * @return        0 on success, -1 on failure
 */
extern int deq_init(struct deque **deq, size_t elesz, void (*destroy)(void *));

/**
 * Destroy all elements, release the internal buffer, and free the deque.
 *
 * @param deq the deque to free, or NULL
 */
extern void deq_free(struct deque *deq);

/**
 * Test whether the deque contains no elements.
 *
 * @param deq the deque to inspect, or NULL
 * @return    non zero if empty, zero otherwise
 */
extern int deq_empty(const struct deque *deq);

/**
 * Return the current number of elements in the deque.
 *
 * @param deq the deque to inspect, or NULL
 * @return    element count, or 0 if deq is NULL
 */
extern size_t deq_size(const struct deque *deq);

/**
 * Return the number of allocated element slots.
 *
 * @param deq the deque to inspect, or NULL
 * @return    capacity in elements, or 0 if deq is NULL
 */
extern size_t deq_capacity(const struct deque *deq);

/**
 * Return a pointer to the front element, equivalent to deq_at(deq, 0).
 *
 * @param deq the deque to inspect, or NULL
 * @return    pointer to the front element, or NULL if deq is NULL or empty
 */
extern void *deq_front(const struct deque *deq);

/**
 * Return a pointer to the back element.
 *
 * @param deq the deque to inspect, or NULL
 * @return    pointer to the back element, or NULL if deq is NULL or empty
 */
extern void *deq_back(const struct deque *deq);

/**
 * Return a pointer to the element at the given logical index.
 *
 * @param deq the deque to inspect, or NULL
 * @param idx zero based logical index
 * @return    pointer to the element, or NULL if deq is NULL or idx is out of
 *            range
 */
extern void *deq_at(const struct deque *deq, size_t idx);

/**
 * Set the logical length of the deque. Growing may reallocate. New slots are
 * zero-filled when capacity grows. Shrinking invokes destroy on each removed
 * element when set.
 *
 * @param deq     the deque to resize
 * @param newsize new element count
 * @return        0 on success, -1 on failure
 */
extern int deq_resize(struct deque *deq, size_t newsize);

/**
 * Reallocate the buffer so capacity matches the current size, releasing unused
 * slots.
 *
 * @param deq the deque to shrink
 * @return    0 on success, -1 on failure
 */
extern int deq_shrink(struct deque *deq);

/**
 * Insert a copy of ele before idx, shifting subsequent elements right. When idx
 * is at or beyond the current size, behaves like deq_pushback. When idx is 0,
 * behaves like deq_pushfront.
 *
 * @param deq the deque to modify
 * @param idx zero based insertion position
 * @param ele pointer to the value to copy, must not be NULL
 * @return    0 on success, -1 on failure
 */
extern int deq_insert(struct deque *deq, size_t idx, void *ele);

/**
 * Remove the element at idx, shifting subsequent elements left. When idx is 0
 * or the last index, behaves like deq_popfront or deq_popback respectively.
 * When dest is non NULL, copy the element there and skip destroy, otherwise
 * invoke destroy when set.
 *
 * @param deq  the deque to modify
 * @param idx  zero based index of the element to remove
 * @param dest buffer of at least elesz bytes to receive the element, or NULL to
 *             invoke destroy
 * @return     0 on success, -1 if deq is NULL or idx is out of range
 */
extern int deq_remove(struct deque *deq, size_t idx, void *dest);

/**
 * Append a copy of ele to the back of the deque, growing the buffer if needed.
 *
 * @param deq the deque to modify
 * @param ele pointer to the value to copy, must not be NULL
 * @return    0 on success, -1 on failure
 */
extern int deq_pushback(struct deque *deq, void *ele);

/**
 * Prepend a copy of ele to the front of the deque, growing the buffer if
 * needed.
 *
 * @param deq the deque to modify
 * @param ele pointer to the value to copy, must not be NULL
 * @return    0 on success, -1 on failure
 */
extern int deq_pushfront(struct deque *deq, void *ele);

/**
 * Remove the back element. When dest is non NULL, copy the element there and
 * skip destroy, otherwise invoke destroy when set.
 *
 * @param deq  the deque to modify
 * @param dest buffer of at least elesz bytes to receive the element, or NULL to
 *             invoke destroy
 * @return     0 on success, -1 if deq is NULL or the deque is empty
 */
extern int deq_popback(struct deque *deq, void *dest);

/**
 * Remove the front element. When dest is non NULL, copy the element there and
 * skip destroy, otherwise invoke destroy when set.
 *
 * @param deq  the deque to modify
 * @param dest buffer of at least elesz bytes to receive the element, or NULL to
 *             invoke destroy
 * @return     0 on success, -1 if deq is NULL or the deque is empty
 */
extern int deq_popfront(struct deque *deq, void *dest);

/**
 * Remove all elements and reset size to zero, invoking destroy on each when
 * set. Does not release the buffer or free the deque itself.
 *
 * @param deq the deque to clear, or NULL
 */
extern void deq_clear(struct deque *deq);

/**
 * Allocate and initialize an iterator positioned at the first logical element
 * of deq.
 *
 * @param iter output pointer to receive the new iterator, must not be NULL
 * @param deq  the deque to traverse, must not be NULL
 * @return     0 on success, -1 on failure
 */
extern int deq_iter_init(struct deque_iter **iter, struct deque *deq);

/**
 * Advance the iterator forward by one logical index.
 *
 * @param iter the iterator to advance, or NULL
 */
extern void deq_iter_inc(struct deque_iter *iter);

/**
 * Move the iterator backward by one logical index.
 *
 * @param iter the iterator to move, or NULL
 */
extern void deq_iter_dec(struct deque_iter *iter);

/**
 * Return a pointer to the element at the current iterator position.
 *
 * @param iter the iterator to read from, or NULL
 * @return     pointer to the current element, or NULL if iter is NULL or the
 *             index is out of range
 */
extern void *deq_iter_get(struct deque_iter *iter);

/**
 * Free the iterator.
 *
 * @param iter the iterator to free, or NULL
 */
extern void deq_iter_free(struct deque_iter *iter);

#ifdef __cplusplus
}
#endif

#endif
