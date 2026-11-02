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

#include <errno.h>
#include <stack.h>
#include <string.h>

int stack_init(struct stack *s, size_t elesz, size_t cap)
{
    return dvec_init(&s->vec, elesz, cap);
}

int stack_init_empty(struct stack *s, size_t elesz)
{
    return dvec_init_empty(&s->vec, elesz);
}

void stack_free(struct stack *s)
{
    if (s == NULL)
        return;
    dvec_free(&s->vec);
}

void stack_clear(struct stack *s) { dvec_clear(&s->vec); }

int stack_push(struct stack *s, const void *ele)
{
    return dvec_pushb(&s->vec, ele);
}

int stack_pop(struct stack *s, void *ele) { return dvec_popb(&s->vec, ele); }

int stack_peek(struct stack *s, void *ele)
{
    void *top;

    top = dvec_back(&s->vec);
    if (!top)
        return -ENOENT;
    if (ele)
        memcpy(ele, top, s->vec.elesz);
    return 0;
}