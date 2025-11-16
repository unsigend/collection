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

#ifndef COLLECTION_STACK_H
#define COLLECTION_STACK_H

/**
 * This file contains the interface for stack.
 */
#include <collection/common.h>
#include <collection/slist.h>
#include <stdbool.h>

// internal struct for Stack
typedef struct {
    SList slist;
} Stack;

/**
 * Check if the stack is empty.
 * @param stack The stack to check.
 * @return True if the stack is empty, false otherwise.
 * @complexity O(1)
 */
#define stack_empty(stack) (slist_empty(&(stack)->slist))

/**
 * Get the size of the stack.
 * @param stack The stack to get the size of.
 * @return The size of the stack.
 * @complexity O(1)
 */
#define stack_size(stack) (slist_size(&(stack)->slist))

/**
 * Initialize the stack.
 * @param stack The stack to initialize.
 * @param destroy The function to destroy the elements.
 * @complexity O(1)
 */
extern void stack_init(Stack* stack, void (*destroy)(void *));

/**
 * Destroy the stack.
 * @param stack The stack to destroy.
 * @complexity O(n)
 */
extern void stack_destroy(Stack* stack);

/**
 * Push an element onto the stack.
 * @param stack The stack to push the element onto.
 * @param data The element to push.
 * @return 0 if successful, -1 if failed
 * @complexity O(1)
 */
extern int stack_push(Stack* stack, void* data);

/**
 * Pop an element from the stack.
 * @param stack The stack to pop the element from.
 * @param data The pointer to store the popped element.
 * @return 0 if successful, -1 if failed
 * @complexity O(1)
 */
extern int stack_pop(Stack* stack, void** data);

/**
 * Peek at the top element of the stack.
 * @param stack The stack to peek at.
 * @return The top element of the stack, or NULL if the stack is empty.
 * @complexity O(1)
 */
extern void* stack_peek(Stack* stack);

/**
 * Clear the stack.
 * @param stack The stack to clear.
 * @complexity O(n)
 */
extern void stack_clear(Stack* stack);

#endif