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
#include <stdlib.h>
#include <vector.h>

struct stack {
    struct vector *vec;
};

int stack_init(struct stack **stack, size_t elesz, void (*destroy)(void *))
{
    if (!stack || !elesz)
        return -1;
    struct stack *s = calloc(1, sizeof(struct stack));
    if (!s)
        return -1;
    int ret = vec_init(&s->vec, elesz, destroy);
    if (ret == -1) {
        free(s);
        return -1;
    }
    *stack = s;
    return 0;
}

void stack_free(struct stack *stack)
{
    if (!stack)
        return;
    vec_free(stack->vec);
    free(stack);
}

int stack_empty(const struct stack *stack)
{
    if (!stack)
        return 1;
    return vec_empty(stack->vec);
}

size_t stack_size(const struct stack *stack)
{
    if (!stack)
        return 0;
    return vec_size(stack->vec);
}

void *stack_peek(const struct stack *stack)
{
    if (!stack)
        return NULL;
    return vec_back(stack->vec);
}

int stack_push(struct stack *stack, void *ele)
{
    if (!stack || !ele)
        return -1;
    return vec_pushback(stack->vec, ele);
}

int stack_pop(struct stack *stack, void *dest)
{
    if (!stack)
        return -1;
    return vec_popback(stack->vec, dest);
}

void stack_clear(struct stack *stack)
{
    if (!stack)
        return;
    vec_clear(stack->vec);
}
