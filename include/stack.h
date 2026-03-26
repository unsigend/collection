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
#include <vector.h>

struct stack;

#define stack_empty(stack)                                                     \
  vec_empty(&(stack)->vec) /* Check if the stack is empty */
#define stack_size(stack)                                                      \
  vec_size(&(stack)->vec) /* Get the size of the stack */

int stack_init(struct stack *stack, size_t elesz, void (*destroy)(void *));
void stack_fini(struct stack *stack);
int stack_push(struct stack *stack, void *ele);
int stack_pop(struct stack *stack, void *dest);
void *stack_peek(struct stack *stack);
void stack_clear(struct stack *stack);

struct stack {
  struct vector vec;
};

#endif