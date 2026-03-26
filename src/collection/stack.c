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

#include <stack.h>
#include <vector.h>

int stack_init(struct stack *stack, size_t elesz, void (*destroy)(void *))
{
  if (!stack || !elesz)
    return -1;
  return vec_init(&stack->vec, elesz, destroy);
}

void stack_fini(struct stack *stack)
{
  if (!stack)
    return;
  vec_fini(&stack->vec);
}

int stack_push(struct stack *stack, void *ele)
{
  if (!stack || !ele)
    return -1;
  return vec_pushback(&stack->vec, ele);
}

int stack_pop(struct stack *stack, void *dest)
{
  if (!stack)
    return -1;
  return vec_popback(&stack->vec, dest);
}

void *stack_peek(struct stack *stack)
{
  if (!stack)
    return NULL;
  return vec_back(&stack->vec);
}

void stack_clear(struct stack *stack)
{
  if (!stack)
    return;
  vec_clear(&stack->vec);
}