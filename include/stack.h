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

#ifndef COL_STACK_H
#define COL_STACK_H

#include <stddef.h>

struct stack;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Allocate and initialize an empty stack. Each stored element is a copy of the
 * value passed to push operations.
 *
 * @param stack   output pointer to receive the new stack, must not be NULL
 * @param elesz   byte size of each element, must be non-zero
 * @param destroy optional callback invoked on each element when it is
 *                discarded, or NULL for no-op
 * @return        0 on success, -1 on failure
 */
int stack_init(struct stack **stack, size_t elesz, void (*destroy)(void *));

/**
 * Destroy all elements, release the internal buffer, and free the stack.
 *
 * @param stack the stack to free, or NULL
 */
void stack_free(struct stack *stack);

/**
 * Test whether the stack contains no elements.
 *
 * @param stack the stack to inspect, or NULL
 * @return      non-zero if empty, zero otherwise
 */
int stack_empty(const struct stack *stack);

/**
 * Return the current number of elements in the stack.
 *
 * @param stack the stack to inspect, or NULL
 * @return      element count, or 0 if stack is NULL
 */
size_t stack_size(const struct stack *stack);

/**
 * Push a copy of ele onto the top of the stack.
 *
 * @param stack the stack to modify
 * @param ele   pointer to the value to copy, must not be NULL
 * @return      0 on success, -1 on failure
 */
int stack_push(struct stack *stack, void *ele);

/**
 * Remove the top element. When dest is non-NULL, copy the element there and
 * skip destroy; otherwise invoke destroy when set.
 *
 * @param stack the stack to modify
 * @param dest  buffer of at least elesz bytes to receive the element, or NULL
 *              to invoke destroy
 * @return      0 on success, -1 if stack is NULL or the stack is empty
 */
int stack_pop(struct stack *stack, void *dest);

/**
 * Return a pointer to the top element without removing it.
 *
 * @param stack the stack to inspect, or NULL
 * @return      pointer to the top element, or NULL if stack is NULL or empty
 */
void *stack_peek(const struct stack *stack);

/**
 * Remove all elements and reset size to zero, invoking destroy on each when
 * set. Does not release the buffer or free the stack.
 *
 * @param stack the stack to clear, or NULL
 */
void stack_clear(struct stack *stack);

#ifdef __cplusplus
}
#endif

#endif
